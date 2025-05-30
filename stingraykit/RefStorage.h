#ifndef STINGRAYKIT_REFSTORAGE_H
#define STINGRAYKIT_REFSTORAGE_H

// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

namespace stingray
{

	template <typename T>
	struct RefStorage
	{
		typedef T ValueType;

		template <typename U>
		static U Wrap(const U& u)   { return u; }

		template <typename U>
		static U Unwrap(const U& u) { return u; }
	};


	template <typename T>
	struct RefStorage<T&>
	{
		typedef T* ValueType;

		template <typename U>
		static U* Wrap(U& u)   { return &u; }

		template <typename U>
		static U& Unwrap(U* u) { return *u; }
	};

}

#endif
