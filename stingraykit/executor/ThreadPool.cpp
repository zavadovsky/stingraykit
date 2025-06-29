// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/executor/ThreadPool.h>

#include <stingraykit/diagnostics/ExecutorsProfiler.h>
#include <stingraykit/function/bind.h>
#include <stingraykit/thread/TimedCancellationToken.h>

namespace stingray
{

	class ThreadPool::WorkerWrapper
	{
	public:
		using CompletedHandler = function<void ()>;

	private:
		std::string				_name;
		optional<TimeDuration>	_profileTimeout;
		optional<TimeDuration>	_idleTimeout;
		ExceptionHandler		_exceptionHandler;
		CompletedHandler		_completedHandler;

		Mutex					_mutex;
		optional<Task>			_task;
		bool					_completed;
		bool					_idle;
		ConditionVariable		_cond;

		Thread					_worker;

	public:
		WorkerWrapper(const std::string& name, optional<TimeDuration> profileTimeout, optional<TimeDuration> idleTimeout, const ExceptionHandler& exceptionHandler, const CompletedHandler& completedHandler, const Task& task)
			:	_name(name),
				_profileTimeout(profileTimeout),
				_idleTimeout(idleTimeout),
				_exceptionHandler(exceptionHandler),
				_completedHandler(completedHandler),
				_task(task),
				_completed(false),
				_idle(false),
				_worker(_name, Bind(&WorkerWrapper::ThreadFunc, this, _1))
		{ }

		bool IsIdle() const
		{
			MutexLock l(_mutex);
			return !_task && _completed && _idle;
		}

		bool CanAddTask() const
		{
			MutexLock l(_mutex);
			return !_task;
		}

		bool TryAddTask(const Task& task)
		{
			MutexLock l(_mutex);
			if (_task)
				return false;

			_task = task;
			_completed = false;
			_idle = false;
			_cond.Broadcast();
			return true;
		}

	private:
		std::string GetProfilerMessage(const Task& task) const
		{ return StringBuilder() % get_function_name(task) % " in ThreadPool '" % _name % "'"; }

		void ThreadFunc(const ICancellationToken& token)
		{
			MutexLock l(_mutex);
			while (token)
			{
				if (!_task)
				{
					if (!_completed)
					{
						MutexUnlock ul(l);
						_completedHandler();
					}

					if (_task)
						continue;

					_completed = true;

					if (!_idle && _idleTimeout)
						_idle = _cond.Wait(_mutex, TimedCancellationToken(token, *_idleTimeout)) == ConditionWaitResult::TimedOut;
					else
						_cond.Wait(_mutex, token);
					continue;
				}

				{
					const Task task = *_task;

					MutexUnlock ul(l);
					ExecuteTask(token, task);
				}

				_task.reset();
			}
		}

		void ExecuteTask(const ICancellationToken& token, const Task& task) const
		{
			try
			{
				if (_profileTimeout)
				{
					AsyncProfiler::Session profilerSession(ExecutorsProfiler::Instance().GetProfiler(), Bind(&WorkerWrapper::GetProfilerMessage, this, wrap_const_ref(task)), *_profileTimeout, AsyncProfiler::NameGetterTag());
					task(token);
				}
				else
					task(token);
			}
			catch (const std::exception& ex)
			{ _exceptionHandler(ex); }
		}
	};


	const TimeDuration ThreadPool::DefaultProfileTimeout = TimeDuration::FromSeconds(10);
	const TimeDuration ThreadPool::DefaultIdleTimeout = TimeDuration::Minute();


	STINGRAYKIT_DEFINE_NAMED_LOGGER(ThreadPool);


	ThreadPool::ThreadPool(const std::string& name, size_t maxThreads, optional<TimeDuration> profileTimeout, optional<TimeDuration> idleTimeout, const ExceptionHandler& exceptionHandler)
		:	_name(name),
			_maxThreads(maxThreads),
			_profileTimeout(profileTimeout),
			_idleTimeout(idleTimeout),
			_exceptionHandler(exceptionHandler)
	{
		STINGRAYKIT_CHECK(_maxThreads != 0, ArgumentException("maxThreads"));
		STINGRAYKIT_CHECK(!_profileTimeout || _profileTimeout >= TimeDuration(), ArgumentException("profileTimeout", _profileTimeout));
		STINGRAYKIT_CHECK(!_idleTimeout || _idleTimeout >= TimeDuration(), ArgumentException("idleTimeout", _idleTimeout));
	}


	ThreadPool::~ThreadPool()
	{ }


	bool ThreadPool::CanQueue() const
	{
		MutexLock l(_mutex);
		if (!_task || (_workers.size() + 1 < _maxThreads))
			return true;

		for (const auto& worker : _workers)
			if (!worker || worker->CanAddTask())
				return true;

		return false;
	}


	void ThreadPool::Queue(const Task& task)
	{ STINGRAYKIT_CHECK(TryQueue(task), "Thread limit exceeded"); }


	bool ThreadPool::TryQueue(const Task& task)
	{
		MutexLock l(_mutex);
		if (!_task)
		{
			if (!_worker)
				_worker = make_unique_ptr<Thread>(_name + "_0", Bind(&ThreadPool::ThreadFunc, this, _1));

			_task = task;
			_cond.Broadcast();
			return true;
		}

		for (Workers::iterator it = _workers.begin(); it != _workers.end(); ++it)
		{
			if (!*it)
			{
				*it = make_unique_ptr<WorkerWrapper>(StringBuilder() % _name % "_" % (std::distance(_workers.begin(), it) + 1), _profileTimeout, _idleTimeout, _exceptionHandler, Bind(&ThreadPool::TaskCompletedHandler, this), task);
				return true;
			}
			else if ((*it)->TryAddTask(task))
				return true;
		}

		if (_workers.size() + 1 >= _maxThreads)
			return false;

		_workers.push_back(make_unique_ptr<WorkerWrapper>(StringBuilder() % _name % "_" % (_workers.size() + 1), _profileTimeout, _idleTimeout, _exceptionHandler, Bind(&ThreadPool::TaskCompletedHandler, this), task));
		return true;
	}


	void ThreadPool::WaitQueue(const Task& task, const ICancellationToken& token)
	{
		MutexLock l(_mutex);

		while (token)
		{
			if (TryQueue(task))
				return;

			switch (_completedCond.Wait(_mutex, token))
			{
			case ConditionWaitResult::Broadcasted:		break;
			case ConditionWaitResult::Cancelled:		STINGRAYKIT_THROW(OperationCancelledException());
			case ConditionWaitResult::TimedOut:			STINGRAYKIT_THROW(TimeoutException());
			}
		}

		STINGRAYKIT_THROW(OperationCancelledException());
	}


	void ThreadPool::DefaultExceptionHandler(const std::exception& ex)
	{ s_logger.Error() << "Uncaught exception:\n" << ex; }


	std::string ThreadPool::GetProfilerMessage(const Task& task) const
	{ return StringBuilder() % get_function_name(task) % " in ThreadPool '" % _name % "'"; }


	void ThreadPool::TaskCompletedHandler()
	{
		MutexLock l(_mutex);
		_completedCond.Broadcast();
	}


	void ThreadPool::ThreadFunc(const ICancellationToken& token)
	{
		MutexLock l(_mutex);
		while (token)
		{
			if (!_task)
			{
				_completedCond.Broadcast();

				if (!_workers.empty() && _idleTimeout)
				{
					if (_cond.Wait(_mutex, TimedCancellationToken(token, *_idleTimeout)) == ConditionWaitResult::TimedOut)
						for (auto& worker : _workers)
							if (worker && worker->IsIdle())
								worker.reset();

					while (!_workers.empty() && !_workers.back())
						_workers.pop_back();
				}
				else
					_cond.Wait(_mutex, token);
				continue;
			}

			{
				const Task task = *_task;

				MutexUnlock ul(l);
				ExecuteTask(token, task);
			}

			_task.reset();
		}
	}

	void ThreadPool::ExecuteTask(const ICancellationToken& token, const Task& task) const
	{
		try
		{
			if (_profileTimeout)
			{
				AsyncProfiler::Session profilerSession(ExecutorsProfiler::Instance().GetProfiler(), Bind(&ThreadPool::GetProfilerMessage, this, wrap_const_ref(task)), *_profileTimeout, AsyncProfiler::NameGetterTag());
				task(token);
			}
			else
				task(token);
		}
		catch (const std::exception& ex)
		{ _exceptionHandler(ex); }
	}

}
