#ifndef STINGRAYKIT_ALGORITHM_H
#define STINGRAYKIT_ALGORITHM_H

// Copyright (c) 2011 - 2015, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/compare/comparers.h>

namespace stingray
{

	template<typename InputIterator, typename OutputIterator, typename Pred>
	OutputIterator copy_if(InputIterator first, InputIterator last, OutputIterator result, Pred pred)
	{
		for (; first != last; ++first)
			if (pred(*first))
				*result++ = *first;
		return result;
	}


	template<class InputIterator, class UnaryPredicate>
	bool any_of (InputIterator first, InputIterator last, UnaryPredicate pred)
	{
		while (first != last)
		{
			if (pred(*first))
				return true;
			++first;
		}
		return false;
	}


	template<class It1, class It2, class Pred>
	bool equal(It1 first1, It1 last1, It2 first2, It2 last2, Pred p)
	{
		for (; first1 != last1; ++first1, ++first2)
			if (first2 == last2 || !p(*first1, *first2))
				return false;
		return first2 == last2;
	}


	template<class It1, class It2>
	bool equal(It1 first1, It1 last1, It2 first2, It2 last2)
	{ return equal(first1, last1, first2, last2, comparers::Equals()); }

}

#endif