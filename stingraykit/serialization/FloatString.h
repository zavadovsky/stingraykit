#ifndef STINGRAYKIT_SERIALIZATION_FLOATSTRING_H
#define STINGRAYKIT_SERIALIZATION_FLOATSTRING_H

// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <string>

namespace stingray
{

	/**
	 * @addtogroup core_settings
	 * @{
	 */

	class FloatString
	{
	private:
		std::string		_str;

	public:
		explicit FloatString(const std::string& str)
			:	_str(str)
		{ }

		explicit FloatString(double value = 0.0);

		std::string ToString() const
		{ return _str; }

		double ToDouble() const;
	};

	/** @} */

}

#endif
