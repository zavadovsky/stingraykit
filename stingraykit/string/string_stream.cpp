// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/string/string_stream.h>

#include <stingraykit/Exception.h>

#include <stdio.h>

namespace stingray
{
	template<>
	template<>
	void basic_string_ostream<char>::Insert(bool value)
	{ Insert(value ? string_view_type("true") : string_view_type("false")); }
	extern template void string_ostream::Insert(bool);

	template<>
	template<typename T>
	void basic_string_ostream<char>::InsertIntegral(T value)
	{
		char buf[32];
		size_t pos = sizeof(buf);
		while (value >= 10)
		{
			const u8 digit = value % 10;
			value /= 10;
			buf[--pos] = digit + '0';
		}
		buf[--pos] = value + '0';
		Insert(string_view_type(buf + pos, sizeof(buf) - pos));
	}

#define DECLARE_INSERT_UNSIGNED(VALUE_TYPE) \
	template<> \
	template<> \
	void basic_string_ostream<char>::Insert(unsigned VALUE_TYPE value) \
	{ InsertIntegral<unsigned VALUE_TYPE>(value); } \
	extern template void string_ostream::Insert(unsigned VALUE_TYPE)

#define DECLARE_INSERT_SIGNED(VALUE_TYPE) \
	template<> \
	template<> \
	void basic_string_ostream<char>::Insert(VALUE_TYPE value) \
	{ \
		if (value < 0) \
		{ \
			push_back('-'); \
			InsertIntegral<unsigned VALUE_TYPE>(-static_cast<unsigned VALUE_TYPE>(value)); \
		} \
		else \
			InsertIntegral<unsigned VALUE_TYPE>(value); \
	} \
	extern template void string_ostream::Insert(VALUE_TYPE)

#define DECLARE_INTEGRAL_INSERT(VALUE_TYPE) \
	DECLARE_INSERT_UNSIGNED(VALUE_TYPE); \
	DECLARE_INSERT_SIGNED(VALUE_TYPE)


#define DECLARE_INSERT_PRINTF(VALUE_TYPE, VALUE_FORMAT, VALUE_FORMAT_TYPE) \
	template<> \
	template<> \
	void basic_string_ostream<char>::Insert(VALUE_TYPE value) \
	{ \
		char buf[32]; \
		const int result = snprintf(buf, sizeof(buf), VALUE_FORMAT, static_cast<VALUE_FORMAT_TYPE>(value)); \
		STINGRAYKIT_CHECK(result >= 0, "snprintf failed for " VALUE_FORMAT); \
		Insert(string_view_type(buf, result)); \
	} \
	extern template void string_ostream::Insert(VALUE_TYPE)


DECLARE_INSERT_UNSIGNED(char);
DECLARE_INTEGRAL_INSERT(short);
DECLARE_INTEGRAL_INSERT(int);
DECLARE_INTEGRAL_INSERT(long);
DECLARE_INTEGRAL_INSERT(long long);

DECLARE_INSERT_PRINTF(long double,	"%.16Lg",	long double);
DECLARE_INSERT_PRINTF(double,		"%.16g",	double);
DECLARE_INSERT_PRINTF(float,		"%.7g",		double);
DECLARE_INSERT_PRINTF(const void*,	"%p",		const void*);

}
