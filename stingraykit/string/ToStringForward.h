#ifndef STINGRAYKIT_STRING_TOSTRINGFORWARD_H
#define STINGRAYKIT_STRING_TOSTRINGFORWARD_H

// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/string/string_stream.h>

namespace stingray
{

	namespace Detail
	{
		template < typename T >
		struct TypeFromStringInterpreter;

		template < typename StringType >
		struct TypeTryFromStringInterpreter;

		template < typename T >
		struct TypeToStringSerializer;
	}


	template < typename T >
	auto FromString(const std::string& str)
			-> decltype(Detail::TypeFromStringInterpreter<T>::template FromStringImpl<T>(str, 0));


	template < typename T >
	auto FromString(string_view str)
			-> decltype(Detail::TypeFromStringInterpreter<T>::template FromStringViewImpl<T>(str, 0));


	template < typename T >
	auto FromString(const char* str)
			-> decltype(Detail::TypeFromStringInterpreter<T>::template FromStringViewImpl<T>(string_view(str), 0));


	template < typename T >
	struct FromStringInterpreter;


	template < typename T >
	auto TryFromString(const std::string& str)
			-> decltype(Detail::TypeTryFromStringInterpreter<T>::template TryFromStringImpl<T>(str, 0));


	template < typename T >
	auto TryFromString(string_view str)
			-> decltype(Detail::TypeTryFromStringInterpreter<T>::template TryFromStringViewImpl<T>(str, 0));


	template < typename T >
	auto TryFromString(const char* str)
			-> decltype(Detail::TypeTryFromStringInterpreter<T>::template TryFromStringViewImpl<T>(string_view(str), 0));


	template < typename T >
	struct TryFromStringInterpreter;


	template < typename T >
	auto ToString(string_ostream& result, const T& val)
			-> decltype(Detail::TypeToStringSerializer<T>::ToStringImpl(result, val, 0), void());


	template < typename T >
	auto ToString(const T& val)
			-> decltype(ToString(std::declval<string_ostream&>(), val), std::string());


	std::string ToString(const std::string& str);
	std::string ToString(string_view str);


	struct ToStringPrinter
	{
		using RetType = std::string;

		template < typename T >
		std::string operator () (const T& val) const;
	};

}

#endif
