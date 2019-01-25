#ifndef STINGRAYKIT_COLLECTION_MAPOBSERVABLEDICTIONARY_H
#define STINGRAYKIT_COLLECTION_MAPOBSERVABLEDICTIONARY_H

// Copyright (c) 2011 - 2019, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/collection/MapDictionary.h>
#include <stingraykit/collection/ObservableDictionaryWrapper.h>

namespace stingray
{

	/**
	 * @addtogroup toolkit_collections
	 * @{
	 */

	template <
			typename KeyType_,
			typename ValueType_ ,
			typename CompareType_ = comparers::Less,
			template <class, class, class, class> class MapType_ = std::map,
			typename AllocatorType_ = std::allocator<std::pair<const KeyType_, ValueType_> >
			>
	struct MapObservableDictionary
		:	public ObservableDictionaryWrapper<MapDictionary<KeyType_, ValueType_, CompareType_, MapType_, AllocatorType_> >
	{
		typedef ObservableDictionaryWrapper<MapDictionary<KeyType_, ValueType_, CompareType_, MapType_, AllocatorType_> > base;

		MapObservableDictionary() : base() { }
		MapObservableDictionary(shared_ptr<IEnumerable<typename base::PairType> > enumerable) : base(enumerable) { }
		MapObservableDictionary(shared_ptr<IEnumerator<typename base::PairType> > enumerator) : base(enumerator) { }
	};

	template <
			typename KeyType,
			typename ValueType,
			typename CompareType = comparers::Less,
			typename AllocatorType = typename flat_map<KeyType, ValueType, CompareType>::allocator_type
			>
	struct FlatMapObservableDictionary
	{ typedef MapObservableDictionary<KeyType, ValueType, CompareType, flat_map, AllocatorType>		Type; };

	/** @} */

}

#endif
