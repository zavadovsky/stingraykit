#ifndef STINGRAYKIT_DIAGNOSTICS_THREADTIMEPROFILER_H
#define STINGRAYKIT_DIAGNOSTICS_THREADTIMEPROFILER_H

// Copyright (c) 2011 - 2017, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.


#include <stingraykit/time/ElapsedTime.h>


namespace stingray
{

	/**
	 * @addtogroup toolkit_profiling
	 * @{
	 */

	STINGRAYKIT_DECLARE_METHOD_CHECK(GetThreadMicroseconds);


	template < typename ThreadEngineType, bool HasGetThreadMicroseconds = HasMethod_GetThreadMicroseconds<ThreadEngineType>::Value >
	class BasicThreadTimeProfiler
	{
	private:
		s64		_start;

	public:
		BasicThreadTimeProfiler()
			: _start(ThreadEngineType::GetThreadMicroseconds())
		{ }

		s64 GetMicroseconds() const { return (s64)ThreadEngineType::GetThreadMicroseconds() - _start; }
	};


	template < typename ThreadEngineType >
	class BasicThreadTimeProfiler<ThreadEngineType, false>
	{
	private:
		ElapsedTime		_timer;

	public:
		BasicThreadTimeProfiler()
		{ }

		s64 GetMicroseconds() const { return _timer.ElapsedMicroseconds(); }
	};


	typedef BasicThreadTimeProfiler<ThreadEngine>		ThreadTimeProfiler;

	/** @} */

}


#endif
