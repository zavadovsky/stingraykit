#ifndef STINGRAYKIT_COLLECTION_FUNCENUMERATOR_H
#define STINGRAYKIT_COLLECTION_FUNCENUMERATOR_H


#include <stingray/toolkit/collection/IEnumerator.h>
#include <stingray/toolkit/function/function.h>


namespace stingray
{

	/**
	 * @addtogroup toolkit_collections
	 * @{
	 */

	template < typename T >
	class FuncEnumerator : public virtual IEnumerator<T>
	{
		typedef function< T(size_t) >		GetFuncType;
		typedef function< bool(size_t) >	ValidFuncType;

	private:
		GetFuncType			_get;
		ValidFuncType		_valid;
		size_t				_index;

	public:
		FuncEnumerator(const GetFuncType& get, const ValidFuncType& valid)
			: _get(get), _valid(valid), _index(0)
		{ }

		virtual bool Valid() const	{ return _valid(_index); }
		virtual T Get() const		{ return _get(_index); }
		virtual void Next()
		{
			if (!_valid(_index))
				return;
			++_index;
		}
	};

	/** @} */

}



#endif
