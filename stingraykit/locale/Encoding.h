#ifndef STINGRAYKIT_LOCALE_ENCODING_H
#define STINGRAYKIT_LOCALE_ENCODING_H

// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/Enum.h>

namespace stingray
{

	/**
	 * @addtogroup toolkit_locale
	 * @{
	 */

	struct Encoding
	{
		STINGRAYKIT_ENUM_VALUES
		(
			ISO_8859_1,
			ISO_8859_2,
			ISO_8859_3,
			ISO_8859_4,
			ISO_8859_5,
			ISO_8859_6,
			ISO_8859_7,
			ISO_8859_8,
			ISO_8859_9,
			ISO_8859_10,
			ISO_8859_11,
			ISO_8859_12,
			ISO_8859_13,
			ISO_8859_14,
			ISO_8859_15,
			ISO_8859_16,
			ISO_10646,
			KSX1001_2004,
			GB_2312_1980,
			ISO_10646_big5,
			ISO_10646_utf8,
			ISO_10646_utf16BE,
			ISO_10646_utf16LE,
			ISO_10646_utf32BE,
			ISO_10646_utf32LE,
			CP1251
		);

		std::string GetName() const
		{
			switch (_enumVal)
			{
			case ISO_8859_1:		return "ISO-8859-1";
			case ISO_8859_2:		return "ISO-8859-2";
			case ISO_8859_3:		return "ISO-8859-3";
			case ISO_8859_4:		return "ISO-8859-4";
			case ISO_8859_5:		return "ISO-8859-5";
			case ISO_8859_6:		return "ISO-8859-6";
			case ISO_8859_7:		return "ISO-8859-7";
			case ISO_8859_8:		return "ISO-8859-8";
			case ISO_8859_9:		return "ISO-8859-9";
			case ISO_8859_10:		return "ISO-8859-10";
			case ISO_8859_11:		return "ISO-8859-11";
			case ISO_8859_12:		return "ISO-8859-12";
			case ISO_8859_13:		return "ISO-8859-13";
			case ISO_8859_14:		return "ISO-8859-14";
			case ISO_8859_15:		return "ISO-8859-15";
			case ISO_8859_16:		return "ISO-8859-16";
			case ISO_10646:			return "ISO-10646";
			case KSX1001_2004:  	return "KSX1001-2004";
			case GB_2312_1980:		return "GB-2312-1980"; //another chinese standard
			case ISO_10646_big5:	return "ISO-10646-big5"; //fixme: find out what is it: big5? big5 was not really supported by anyone
			case ISO_10646_utf8:	return "UTF-8";
			case ISO_10646_utf16BE:	return "UTF-16BE";
			case ISO_10646_utf16LE:	return "UTF-16LE";
			case ISO_10646_utf32BE:	return "UTF-32BE";
			case ISO_10646_utf32LE:	return "UTF-32LE";
			case CP1251:			return "CP1251";
			default:				return "Unknown";
			}
		}

		STINGRAYKIT_DECLARE_ENUM_CLASS(Encoding);
	};

	/** @} */

}

#endif
