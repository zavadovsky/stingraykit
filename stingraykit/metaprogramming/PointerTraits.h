#ifndef STINGRAYKIT_METAPROGRAMMING_POINTERTRAITS_H
#define STINGRAYKIT_METAPROGRAMMING_POINTERTRAITS_H

// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/metaprogramming/CVTraits.h>
#include <stingraykit/metaprogramming/ReferenceTraits.h>

namespace stingray
{

	namespace Detail
	{

		template < typename T > struct IsPointerImpl								: FalseType	{ };
		template < typename T > struct IsPointerImpl<T*>							: TrueType	{ };

	}
	template < typename T > struct IsPointer										: Detail::IsPointerImpl<typename RemoveCV<T>::ValueT> { };

	template < typename T > struct RemovePointer									{ typedef T ValueT; };
	template < typename T > struct RemovePointer<T*>								{ typedef T ValueT; };
	template < typename T > struct RemovePointer<T* const>							{ typedef T ValueT; };
	template < typename T > struct RemovePointer<T* volatile>						{ typedef T ValueT; };
	template < typename T > struct RemovePointer<T* const volatile>					{ typedef T ValueT; };

	template < typename T > struct AddPointer										{ typedef typename RemoveReference<T>::ValueT* ValueT; };

	template < typename T > struct ToPointer										{ typedef T* ValueT; };
	template < typename T > struct ToPointer<T&>									{ typedef T* ValueT; };
	template < typename T > struct ToPointer<T*>									{ typedef T* ValueT; };
	template < typename T > struct ToPointer<T* const>								{ typedef T* ValueT; };
	template < typename T > struct ToPointer<T* volatile>							{ typedef T* ValueT; };
	template < typename T > struct ToPointer<T* const volatile>						{ typedef T* ValueT; };

}

#endif
