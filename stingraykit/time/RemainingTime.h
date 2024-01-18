// Copyright (c) 2011 - 2024, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#ifndef STINGRAYKIT_TIME_REMAININGTIME_H
#define STINGRAYKIT_TIME_REMAININGTIME_H

#include <stingraykit/time/ElapsedTime.h>

namespace stingray
{

	class RemainingTime
	{
	private:
		TimeDuration	_timeout;
		ElapsedTime		_elapsed;

	public:
		explicit RemainingTime(TimeDuration timeout) : _timeout(timeout) { }

		void Restart()					{ _elapsed.Restart(); }

		TimeDuration Remaining() const
		{
			const TimeDuration elapsed = _elapsed.Elapsed();
			return _timeout > elapsed ? _timeout - elapsed : TimeDuration();
		}

		bool Expired() const			{ return _timeout <= _elapsed.Elapsed(); }
	};

}

#endif
