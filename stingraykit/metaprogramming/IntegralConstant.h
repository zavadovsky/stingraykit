#ifndef STINGRAYKIT_METAPROGRAMMING_INTEGRALCONSTANT_H
#define STINGRAYKIT_METAPROGRAMMING_INTEGRALCONSTANT_H

// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

namespace stingray
{

	template < typename T_, T_ Value_ >
	struct integral_constant
	{
		using ValueT = T_;

		static const ValueT Value = Value_;
	};

	template < typename T_, T_ Value_ >
	const T_ integral_constant<T_, Value_>::Value;


	using TrueType = integral_constant<bool, true>;
	using FalseType = integral_constant<bool, false>;


	template < typename Val1, typename Val2 >
	struct integral_constant_less : integral_constant<bool, Val1::Value < Val2::Value> { };

}

#endif
