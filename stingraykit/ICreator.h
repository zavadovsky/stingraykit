#ifndef STINGRAYKIT_ICREATOR_H
#define STINGRAYKIT_ICREATOR_H

// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/shared_ptr.h>

#define STINGRAYKIT_DECLARE_CREATOR(ClassName, ...) \
		using ClassName##Creator = stingray::ICreator<ClassName, ##__VA_ARGS__>; \
		STINGRAYKIT_DECLARE_PTR(ClassName##Creator)

namespace stingray
{

	template < typename T, typename... Args >
	struct ICreator
	{
		virtual ~ICreator() { }

		virtual shared_ptr<T> Create(const Args&... args) const = 0;
	};

}

#endif
