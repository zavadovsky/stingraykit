#ifndef STINGRAYKIT_THREAD_CONDITIONVARIABLE_H
#define STINGRAYKIT_THREAD_CONDITIONVARIABLE_H

// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.


#include <stingraykit/thread/Thread.h>

#if PROFILE_MUTEX_LOCK_TIMINGS
#	include <stingraykit/thread/BasicConditionVariable.h>
namespace stingray { typedef BasicConditionVariable<Mutex, Semaphore>	ConditionVariable; }
#elif PLATFORM_POSIX
#	include <stingraykit/thread/posix/PosixConditionVariable.h>
namespace stingray { typedef PosixConditionVariable						ConditionVariable; }
#else
#	include <stingraykit/thread/BasicConditionVariable.h>
namespace stingray { typedef BasicConditionVariable<Mutex, Semaphore>	ConditionVariable; }
#endif


#endif
