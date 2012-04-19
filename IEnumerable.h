#ifndef __GS_DVRLIB_TOOLKIT_IENUMERABLE_H__
#define __GS_DVRLIB_TOOLKIT_IENUMERABLE_H__


#include <stingray/toolkit/IEnumerator.h>

#define TOOLKIT_DECLARE_ENUMERABLE(ClassName) \
		typedef stingray::IEnumerable<ClassName>				ClassName##Enumerable; \
		TOOLKIT_DECLARE_PTR(ClassName##Enumerable); \
		TOOLKIT_DECLARE_ENUMERATOR(ClassName)

namespace stingray
{


	template < typename T >
	struct IEnumerable
	{
		typedef T ItemType;

		virtual ~IEnumerable() { }

		virtual shared_ptr<IEnumerator<T> > GetEnumerator() const = 0;
	};


	template < typename T >
	struct EmptyEnumerable : public virtual IEnumerable<T>
	{
		virtual shared_ptr<IEnumerator<T> > GetEnumerator() const
		{ return make_shared<EmptyEnumerator<T> >(); }
	};


	class EmptyEnumerableProxy
	{
	public:
		template< typename U >
		operator shared_ptr<IEnumerable<U> >() const
		{ return make_shared<EmptyEnumerable<U> >(); }
	};


	inline EmptyEnumerableProxy MakeEmptyEnumerable()
	{ return EmptyEnumerableProxy(); }


	template < typename T >
	struct OneItemEnumerable : public virtual IEnumerable<T>
	{
	private:
		T		_value;

	public:
		OneItemEnumerable(typename GetConstReferenceType<T>::ValueT value)
			: _value(value)
		{ }

		virtual shared_ptr<IEnumerator<T> > GetEnumerator() const
		{ return make_shared<OneItemEnumerator<T> >(_value); }
	};


	template< typename T >
	class OneItemEnumerableProxy
	{
	private:
		T	_item;

	public:
		explicit OneItemEnumerableProxy(const T& item)
			: _item(item)
		{ }

		template< typename U >
		operator shared_ptr<IEnumerable<U> >() const
		{ return make_shared<OneItemEnumerable<U> >(_item); }
	};


	template< typename T >
	OneItemEnumerableProxy<T> MakeOneItemEnumerable(const T& item)
	{ return OneItemEnumerableProxy<T>(item); }


}


#endif
