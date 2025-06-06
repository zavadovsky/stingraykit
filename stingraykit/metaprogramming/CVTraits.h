#ifndef STINGRAYKIT_METAPROGRAMMING_CVTRAITS_H
#define STINGRAYKIT_METAPROGRAMMING_CVTRAITS_H

// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/metaprogramming/IntegralConstant.h>

namespace stingray
{

	template < typename T > struct IsConst											: FalseType	{ };
	template < typename T > struct IsConst<const T>									: TrueType	{ };

	template < typename T > struct IsVolatile										: FalseType	{ };
	template < typename T > struct IsVolatile<volatile T>							: TrueType	{ };

	template < typename T > struct RemoveConst										{ typedef T ValueT; };
	template < typename T > struct RemoveConst<const T>								{ typedef T ValueT; };

	template < typename T > struct AddConst											{ typedef const T ValueT; };

	template < typename T > struct RemoveVolatile									{ typedef T ValueT; };
	template < typename T > struct RemoveVolatile<volatile T>						{ typedef T ValueT; };

	template < typename T > struct AddVolatile										{ typedef volatile T ValueT; };

	template < typename T > struct RemoveCV											{ typedef typename RemoveVolatile<typename RemoveConst<T>::ValueT>::ValueT ValueT; };
	template < typename T > struct AddCV											{ typedef const volatile T ValueT; };

}

#endif
