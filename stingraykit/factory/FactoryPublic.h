#ifndef STINGRAYKIT_FACTORY_FACTORYPUBLIC_H
#define STINGRAYKIT_FACTORY_FACTORYPUBLIC_H

// Copyright (c) 2011 - 2024, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/factory/IFactoryObject.h>

namespace stingray
{

	namespace Detail
	{

		template < typename ClassType >
		class FactoryObjectCreator;

	}


	struct FactoryUtils
	{
		static std::string RemoveTypeSuffix(const std::string& type, const std::string& suffix);
		static std::string RemoveTypePrefix(const std::string& type, const std::string& prefix);
	};

}


#define STINGRAYKIT_REGISTER_CLASS(Class_) \
		std::string GetClassName() const override \
		{ return stingray::FactoryUtils::RemoveTypePrefix(TypeInfo(typeid(Class_)).GetName(), "stingray::"); } \
		friend class stingray::Detail::FactoryObjectCreator<Class_>

#endif