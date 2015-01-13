#ifndef STINGRAYKIT_ICLONEABLE_H
#define STINGRAYKIT_ICLONEABLE_H


#include <stingray/toolkit/shared_ptr.h>


namespace stingray
{

	template < typename T >
	struct ICloneable
	{
		virtual ~ICloneable() { }

		virtual shared_ptr<T> Clone() const = 0;
	};

}


#endif
