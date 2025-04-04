// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/timer/Timer.h>

#include <stingraykit/diagnostics/ExecutorsProfiler.h>
#include <stingraykit/function/CancellableFunction.h>
#include <stingraykit/function/bind.h>
#include <stingraykit/thread/TimedCancellationToken.h>
#include <stingraykit/FunctionToken.h>

namespace stingray
{

	class Timer::CallbackQueue
	{
		using Container = std::multimap<TimeDuration, CallbackInfoPtr>;

	private:
		Mutex			_mutex;
		Container		_container;

	public:
		using iterator = Container::const_iterator;

		Mutex& Sync()
		{ return _mutex; }

		bool IsEmpty() const
		{
			MutexLock l(_mutex);
			return _container.empty();
		}

		CallbackInfoPtr Top() const;
		void Push(const CallbackInfoPtr& ci);
		void Erase(const CallbackInfoPtr& ci);
		CallbackInfoPtr Pop();
	};


	class Timer::CallbackInfo
	{
		STINGRAYKIT_NONCOPYABLE(CallbackInfo);

		friend class CallbackQueue;

	private:
		using QueueIterator = CallbackQueue::iterator;

	private:
		optional<TaskType>			_task;
		TimeDuration				_timeToTrigger;
		optional<TimeDuration>		_period;
		TaskLifeToken				_token;

		bool						_erased;
		optional<QueueIterator>		_iterator;

	public:
		CallbackInfo(const TaskType& task, TimeDuration timeToTrigger, optional<TimeDuration> period, TaskLifeToken&& token)
			:	_task(task),
				_timeToTrigger(timeToTrigger),
				_period(period),
				_token(std::move(token)),
				_erased(false)
		{ }

		const TaskType& GetTask() const							{ return *STINGRAYKIT_REQUIRE_INITIALIZED(_task); }
		FutureExecutionTester GetExecutionTester() const 		{ return _token.GetExecutionTester(); }
		void Release()
		{
			_token.Release();
			_task.reset();
		}

		bool IsPeriodic() const									{ return _period.is_initialized(); }
		void Restart(TimeDuration currentTime)					{ _timeToTrigger = currentTime + *STINGRAYKIT_REQUIRE_INITIALIZED(_period); }
		TimeDuration GetTimeToTrigger() const					{ return _timeToTrigger; }

	private:
		void SetIterator(const optional<QueueIterator>& it)		{ _iterator = it; }
		const optional<QueueIterator>& GetIterator() const		{ return _iterator; }
		void SetErased()										{ _erased = true; }
		bool IsErased() const									{ return _erased; }
	};


	Timer::CallbackInfoPtr Timer::CallbackQueue::Top() const
	{
		MutexLock l(_mutex);
		STINGRAYKIT_CHECK(!_container.empty(), "Empty queue");
		return _container.begin()->second;
	}


	void Timer::CallbackQueue::Push(const CallbackInfoPtr& ci)
	{
		MutexLock l(_mutex);
		if (!ci->IsErased())
			ci->SetIterator(iterator(_container.emplace(ci->GetTimeToTrigger(), ci)));
	}


	void Timer::CallbackQueue::Erase(const CallbackInfoPtr& ci)
	{
		MutexLock l(_mutex);
		ci->SetErased();

		if (!ci->GetIterator())
			return;

		_container.erase(*ci->GetIterator());
		ci->SetIterator(null);
	}


	Timer::CallbackInfoPtr Timer::CallbackQueue::Pop()
	{
		MutexLock l(_mutex);
		STINGRAYKIT_CHECK(!_container.empty(), "Empty queue");

		const CallbackInfoPtr ci = _container.begin()->second;

		_container.erase(_container.begin());
		ci->SetIterator(null);

		return ci;
	}


	const TimeDuration Timer::DefaultProfileTimeout = TimeDuration::FromSeconds(10);


	STINGRAYKIT_DEFINE_NAMED_LOGGER(Timer);


	Timer::Timer(const std::string& timerName, optional<TimeDuration> profileTimeout, const ExceptionHandler& exceptionHandler)
		:	_timerName(timerName),
			_profileTimeout(profileTimeout),
			_exceptionHandler(exceptionHandler),
			_queue(make_shared_ptr<CallbackQueue>()),
			_worker(make_shared_ptr<Thread>(timerName, Bind(&Timer::ThreadFunc, this, _1)))
	{ }


	Timer::~Timer()
	{
		_worker.reset();

		MutexLock l(_queue->Sync());
		if (!_queue->IsEmpty())
			s_logger.Warning() << "killing timer " << _timerName << " which still has some functions to execute";

		while(!_queue->IsEmpty())
		{
			CallbackInfoPtr top = _queue->Pop();
			MutexUnlock ul(l);
			top.reset();
		}
	}


	void Timer::AddTask(const TaskType& task, const FutureExecutionTester& tester)
	{
		const CallbackInfoPtr ci = make_shared_ptr<CallbackInfo>(MakeCancellableFunction(task, tester), _monotonic.Elapsed(), null, TaskLifeToken::CreateDummyTaskToken());

		MutexLock l(_queue->Sync());
		_queue->Push(ci);
		_cond.Broadcast();
	}


	Token Timer::SetTimeout(TimeDuration timeout, const TaskType& task)
	{
		const CallbackInfoPtr ci = make_shared_ptr<CallbackInfo>(task, _monotonic.Elapsed() + timeout, null, TaskLifeToken());
		const Token token = MakeFunctionToken(Bind(&Timer::RemoveTask, _queue, ci));

		{
			MutexLock l(_queue->Sync());
			_queue->Push(ci);
			_cond.Broadcast();
		}

		return token;
	}


	Token Timer::SetTimer(TimeDuration interval, const TaskType& task)
	{ return SetTimer(interval, interval, task); }


	Token Timer::SetTimer(TimeDuration timeout, TimeDuration interval, const TaskType& task)
	{
		const CallbackInfoPtr ci = make_shared_ptr<CallbackInfo>(task, _monotonic.Elapsed() + timeout, interval, TaskLifeToken());
		const Token token = MakeFunctionToken(Bind(&Timer::RemoveTask, _queue, ci));

		{
			MutexLock l(_queue->Sync());
			_queue->Push(ci);
			_cond.Broadcast();
		}

		return token;
	}


	void Timer::RemoveTask(const CallbackQueuePtr& queue, const CallbackInfoPtr& ci)
	{
		{
			MutexLock l(queue->Sync());
			queue->Erase(ci);
		}
		ci->Release();
	}


	void Timer::DefaultExceptionHandler(const std::exception& ex)
	{ s_logger.Error() << "Timer task exception: " << ex; }


	std::string Timer::GetProfilerMessage(const TaskType& task) const
	{ return StringBuilder() % get_function_name(task) % " in Timer '" % _timerName % "'"; }


	void Timer::ThreadFunc(const ICancellationToken& token)
	{
		MutexLock l(_queue->Sync());

		while (token)
		{
			if (_queue->IsEmpty())
			{
				_cond.Wait(_queue->Sync(), token);
				continue;
			}

			CallbackInfoPtr top = _queue->Top();
			if (top->GetTimeToTrigger() <= _monotonic.Elapsed())
			{
				_queue->Pop();

				MutexUnlock ul(l);

				const optional<TimeDuration> monotonic = top->IsPeriodic() ? make_optional_value(_monotonic.Elapsed()) : null;

				ExecuteTask(top);

				if (monotonic)
				{
					top->Restart(*monotonic);
					_queue->Push(top);
				}

				top.reset();
			}
			else //top timer not triggered
			{
				const TimeDuration waitTime = top->GetTimeToTrigger() - _monotonic.Elapsed();
				top.reset();
				if (waitTime > TimeDuration())
					_cond.Wait(_queue->Sync(), TimedCancellationToken(token, waitTime));
			}
		}

		const TimeDuration currentTime = _monotonic.Elapsed();
		while (!_queue->IsEmpty())
		{
			CallbackInfoPtr top = _queue->Top();

			if (top->GetTimeToTrigger() > currentTime)
				break;

			_queue->Pop();

			MutexUnlock ul(l);

			ExecuteTask(top);
			top.reset();
		}
	}


	void Timer::ExecuteTask(const CallbackInfoPtr& ci) const
	{
		try
		{
			LocalExecutionGuard guard(ci->GetExecutionTester());
			if (!guard)
				return;

			if (_profileTimeout)
			{
				AsyncProfiler::Session profiler_session(ExecutorsProfiler::Instance().GetProfiler(), Bind(&Timer::GetProfilerMessage, this, wrap_const_ref(ci->GetTask())), *_profileTimeout, AsyncProfiler::NameGetterTag());
				ci->GetTask()();
			}
			else
				ci->GetTask()();
		}
		catch (const std::exception& ex)
		{ _exceptionHandler(ex); }
	}


	////////////////////////////////////////////////////////////////////////////////


	void ExecutionDeferrer::Cancel()
	{
		{
			MutexLock l(_mutex);
			if (_cancellationActive)
				return;
			_cancellationActive = true;
		}

		{
			MutexLock l(_doDeferConnectionMutex);
			_doDeferConnection.Reset();
		}
		{
			MutexLock l(_connectionMutex);
			_connection.Reset();
		}

		MutexLock l(_mutex);
		_cancellationActive = false;
	}


	void ExecutionDeferrer::Defer(const TaskType& task)
	{
		STINGRAYKIT_CHECK(_timeout != TimeDuration(), "Invalid timeout");
		Defer(task, _timeout);
	}


	void ExecutionDeferrer::Defer(const TaskType& task, TimeDuration timeout, optional<TimeDuration> interval)
	{
		MutexLock l(_doDeferConnectionMutex);
		_doDeferConnection = _timer.SetTimeout(TimeDuration(), Bind(&ExecutionDeferrer::DoDefer, this, task, timeout, interval));
	}


	void ExecutionDeferrer::DoDefer(const TaskType& task, TimeDuration timeout, optional<TimeDuration> interval)
	{
		MutexLock l(_connectionMutex);
		if (interval)
			_connection = _timer.SetTimer(timeout, *interval, task);
		else
			_connection = _timer.SetTimeout(timeout, task);
	}

}
