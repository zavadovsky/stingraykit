#ifndef STINGRAYKIT_COLLECTION_FOREACH_H
#define STINGRAYKIT_COLLECTION_FOREACH_H

// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/collection/IEnumerable.h>
#include <stingraykit/RefStorage.h>

namespace stingray
{

	/**
	 * @addtogroup toolkit_collections
	 * @{
	 *
	 * @brief A foreach loop
	 * @param[in] ... &lt;type&gt; &lt;name&gt; IN &lt;enumerable or enumerator&gt; [ WHERE &lt;some condition&gt; ]
	 * @par Example:
	 * @code
	 * FOR_EACH(ISomeObjectPtr obj IN something->GetObjectCollection())
	 * {
	 *     ISomeOtherObjectPtr other_obj = dynamic_pointer_cast<ISomeOtherObject>(obj);
	 *     if (!other_obj)
	 *         continue;
	 *     if (other_obj->GetProperty() == 1)
	 *         continue;
	 *     other_obj->SomeMethod();
	 * }
	 * // or
	 * FOR_EACH(ISomeOtherObjectPtr other_obj IN something->GetObjectCollection() WHERE other_obj->GetProperty() != 1)
	 *     other_obj->SomeMethod();
	 * @endcode
	 */

#define FOR_EACH(...) FOR_EACH__IMPL(__VA_ARGS__)


	namespace Detail
	{

		template < typename Signature >
		struct GetItemTypeFromItemDecl;

		template < typename T >
		struct GetItemTypeFromItemDecl<void (*) (T)>
		{ using ValueT = T; };

		template < typename T >
		bool ForEachItemFilter(bool dummy, const T& val) { return static_cast<bool>(val); }
		inline bool ForEachItemFilter(bool dummy) { return true; }

		template < typename DestType, typename SrcEnumeratorPtr >
		class ForEachEnumeratorCaster : public virtual IEnumerator<DestType>
		{
			using Storage = RefStorage<DestType>;

		private:
			SrcEnumeratorPtr				_srcEnumerator;
			typename Storage::ValueType		_dst;

		public:
			ForEachEnumeratorCaster(const SrcEnumeratorPtr& srcEnumerator) : _srcEnumerator(STINGRAYKIT_REQUIRE_NOT_NULL(srcEnumerator))
			{ FindNext(); }

			bool Valid() const override
			{ return _srcEnumerator->Valid(); }

			DestType Get() const override
			{
				STINGRAYKIT_CHECK(_srcEnumerator->Valid(), "Enumerator is not valid");
				return Storage::Unwrap(_dst);
			}

			void Next() override
			{
				_srcEnumerator->Next();
				FindNext();
			}

		private:
			void FindNext()
			{
				for (; _srcEnumerator->Valid(); _srcEnumerator->Next())
				{
					_dst = dynamic_caster(Storage::Wrap(_srcEnumerator->Get()));
					if (_dst)
						break;
				}
			}
		};

		template < typename SrcType >
		class ForEachEnumeratorCasterProxy
		{
			using SrcEnumeratorPtr = shared_ptr<IEnumerator<SrcType>>;

		private:
			SrcEnumeratorPtr				_srcEnumerator;

		public:
			ForEachEnumeratorCasterProxy(const SrcEnumeratorPtr& srcEnumerator) : _srcEnumerator(STINGRAYKIT_REQUIRE_NOT_NULL(srcEnumerator))
			{ }

			operator SrcEnumeratorPtr () const
			{ return _srcEnumerator; }

			template < typename DestType >
			operator shared_ptr<IEnumerator<DestType>> () const
			{ return make_shared_ptr<ForEachEnumeratorCaster<DestType, SrcEnumeratorPtr>>(_srcEnumerator); }
		};

		template < typename SrcEnumeratorType >
		ForEachEnumeratorCasterProxy<typename SrcEnumeratorType::ItemType> GetForEachEnumeratorCaster(const shared_ptr<SrcEnumeratorType>& enumerator)
		{ return ForEachEnumeratorCasterProxy<typename SrcEnumeratorType::ItemType>(enumerator); }

	}


#define IN ,
#define WHERE ,
#define FOR_EACH__IMPL(ItemDecl_, SomethingToEnumerate_, ...) \
		for (bool __broken__ = false; !__broken__; __broken__ = true) \
			for (::stingray::shared_ptr<::stingray::IEnumerator<typename ::stingray::Detail::GetItemTypeFromItemDecl<void (*) (ItemDecl_)>::ValueT>> __en__(::stingray::Detail::GetForEachEnumeratorCaster(::stingray::ToEnumerator(SomethingToEnumerate_))); \
					__en__ && __en__->Valid() && !__broken__; \
					__en__->Next()) \
				 for (bool __dummy_bool__ = true; __dummy_bool__ && !__broken__; ) \
					 for (ItemDecl_ = __en__->Get(); __dummy_bool__ && ((__dummy_bool__ = false) == false) && ::stingray::Detail::ForEachItemFilter(true, ##__VA_ARGS__) && (__broken__ = true); __broken__ = false)

	/** @} */

}

#endif
