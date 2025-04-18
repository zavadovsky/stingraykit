// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/thread/ThreadOperation.h>

#include <stingraykit/thread/posix/ThreadLocal.h>

namespace stingray
{

	namespace
	{

		STINGRAYKIT_DECLARE_THREAD_LOCAL(int, RestrictedThreadOperations);
		STINGRAYKIT_DEFINE_THREAD_LOCAL(int, RestrictedThreadOperations);

		STINGRAYKIT_DECLARE_THREAD_LOCAL(int, ExclusiveThreadOperations);
		STINGRAYKIT_DEFINE_THREAD_LOCAL(int, ExclusiveThreadOperations);

	}


	STINGRAYKIT_DEFINE_NAMED_LOGGER(ThreadOperationConstrainer);


	ThreadOperationConstrainer::ThreadOperationConstrainer(ThreadOperation operations)
		:	_operations(operations),
			_threadId(ThreadEngine::GetCurrentThreadId()),
			_oldValue(RestrictedThreadOperations::Get())
	{ RestrictedThreadOperations::Get() = _oldValue | _operations.val(); }


	ThreadOperationConstrainer::~ThreadOperationConstrainer()
	{ STINGRAYKIT_TRY_NO_MESSAGE(Unconstrain()); }


	void ThreadOperationConstrainer::Unconstrain()
	{
		if (_threadId == ThreadEngine::GetCurrentThreadId())
			RestrictedThreadOperations::Get() = _oldValue;
		else
		{
			s_logger.Error() << "Can't unconstrain " << _operations << " operations that was constrained in another thread\nBacktrace: " << Backtrace();
#ifndef PRODUCTION_BUILD
			STINGRAYKIT_FATAL("invalid unconstraining of operations");
#endif
		}
	}


	STINGRAYKIT_DEFINE_NAMED_LOGGER(ThreadOperationReporter);


	ThreadOperationReporter::ThreadOperationReporter(ThreadOperation op)
	{
		if (op.val() & RestrictedThreadOperations::Get())
			s_logger.Error() << op << " operations are prohibited in this thread\nBacktrace: " << Backtrace();
	}


	STINGRAYKIT_DEFINE_NAMED_LOGGER(ExclusiveThreadOperation);


	ExclusiveThreadOperation::ExclusiveThreadOperation(ThreadOperation operations)
		:	_operations(operations),
			_threadId(ThreadEngine::GetCurrentThreadId()),
			_oldValue(ExclusiveThreadOperations::Get())
	{ ExclusiveThreadOperations::Get() = _oldValue | _operations.val(); }


	ExclusiveThreadOperation::~ExclusiveThreadOperation()
	{ STINGRAYKIT_TRY_NO_MESSAGE(Unexclusivize()); }


	void ExclusiveThreadOperation::Unexclusivize()
	{
		if (_threadId == ThreadEngine::GetCurrentThreadId())
			ExclusiveThreadOperations::Get() = _oldValue;
		else
		{
			s_logger.Error() << "Can't unexclusivize " << _operations << " operations that was exclusivized in another thread\nBacktrace: " << Backtrace();
#ifndef PRODUCTION_BUILD
			STINGRAYKIT_FATAL("invalid unexclusivizing of operations");
#endif
		}
	}


	STINGRAYKIT_DEFINE_NAMED_LOGGER(ExclusiveThreadOperationChecker);


	ExclusiveThreadOperationChecker::ExclusiveThreadOperationChecker(ThreadOperation op)
	{
		if (!(op.val() & ExclusiveThreadOperations::Get()))
		{
			s_logger.Error() << op << " operations happened out of exclusive operation area: " << Backtrace();
#ifndef PRODUCTION_BUILD
			STINGRAYKIT_FATAL("forbidden exclusive operation");
#endif
		}
	}

}
