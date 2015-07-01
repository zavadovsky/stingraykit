#ifndef STINGRAYKIT_STRING_UNICODE_H
#define STINGRAYKIT_STRING_UNICODE_H

// Copyright (c) 2011 - 2015, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.


#include <stingraykit/toolkit.h>
#include <stingraykit/shared_ptr.h>

#ifdef HAVE_ICU_I18N
#	include <unicode/coll.h>
#endif


namespace stingray
{

	class UnicodeCollator
	{
		STINGRAYKIT_NONCOPYABLE(UnicodeCollator);

	private:
#ifdef HAVE_ICU_I18N
		unique_ptr<icu::Collator>	_collator;
#endif
		bool						_caseSensitive;

	public:
		UnicodeCollator();
		~UnicodeCollator();

		void SetCaseSensitivity(bool sensitive);
		int Compare(const std::string &str1, const std::string &str2) const;
	};
	STINGRAYKIT_DECLARE_PTR(UnicodeCollator);


	std::string Utf8ToLower(const std::string& str);

}


#endif
