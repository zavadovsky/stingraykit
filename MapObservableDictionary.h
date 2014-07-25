#ifndef STINGRAY_TOOLKIT_MAPOBSERVABLEDICTIONARY_H
#define STINGRAY_TOOLKIT_MAPOBSERVABLEDICTIONARY_H


#include <stingray/toolkit/IObservableDictionary.h>
#include <stingray/toolkit/MapDictionary.h>


namespace stingray
{

	/**
	 * @addtogroup toolkit_collections
	 * @{
	 */

	template < typename KeyType_, typename ValueType_, typename CompareType_ = std::less<KeyType_> >
	class MapObservableDictionary : public ObservableDictionaryWrapper<MapDictionary<KeyType_, ValueType_, CompareType_> >
	{ };

	/** @} */

}


#endif
