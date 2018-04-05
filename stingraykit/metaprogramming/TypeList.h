#ifndef STINGRAYKIT_METAPROGRAMMING_TYPELIST_H
#define STINGRAYKIT_METAPROGRAMMING_TYPELIST_H

// Copyright (c) 2011 - 2017, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.


#include <sys/types.h>

#include <stingraykit/Macro.h>
#include <stingraykit/metaprogramming/Functional.h>
#include <stingraykit/metaprogramming/If.h>
#include <stingraykit/metaprogramming/YesNo.h>


namespace stingray
{


	struct TypeListEndNode
	{ typedef TypeListEndNode type; };


	template < typename Value_, typename Next_ >
	struct TypeListNode
	{
		typedef Value_	ValueT;
		typedef Next_	Next;
	};


	//////////////////////////////////////////////////////////////////////


#define TY typename

#define MAX_TYPELIST_LEN 60

#define DETAIL_TYPELIST_PARAMS_DECL(Index_, Default_) STINGRAYKIT_COMMA_IF(Index_) typename STINGRAYKIT_CAT(T, Index_) Default_
#define DETAIL_TYPELIST_PARAMS_USAGE(Index_, Shift_) STINGRAYKIT_COMMA_IF(Index_) STINGRAYKIT_CAT(T, STINGRAYKIT_ADD(Index_, Shift_))
#define DETAIL_TYPELISTENDNODE(Index_, unused) STINGRAYKIT_COMMA_IF(Index_) TypeListEndNode

	namespace Detail
	{
		template< STINGRAYKIT_REPEAT(MAX_TYPELIST_LEN, DETAIL_TYPELIST_PARAMS_DECL, STINGRAYKIT_EMPTY()) >
		struct TypeListCreatorImpl
		{
			typedef TypeListNode<T0, typename TypeListCreatorImpl<STINGRAYKIT_REPEAT(STINGRAYKIT_DEC(MAX_TYPELIST_LEN), DETAIL_TYPELIST_PARAMS_USAGE, 1), TypeListEndNode>::ValueT > ValueT;
		};

		template<>
		struct TypeListCreatorImpl<STINGRAYKIT_REPEAT(MAX_TYPELIST_LEN, DETAIL_TYPELISTENDNODE, ~)>
		{
			typedef TypeListEndNode ValueT;
		};
	}

	template< STINGRAYKIT_REPEAT(MAX_TYPELIST_LEN, DETAIL_TYPELIST_PARAMS_DECL, = TypeListEndNode) >
	struct TypeList : public Detail::TypeListCreatorImpl< STINGRAYKIT_REPEAT(MAX_TYPELIST_LEN, DETAIL_TYPELIST_PARAMS_USAGE, 0) >::ValueT
	{
		typedef typename Detail::TypeListCreatorImpl< STINGRAYKIT_REPEAT(MAX_TYPELIST_LEN, DETAIL_TYPELIST_PARAMS_USAGE, 0) >::ValueT type;
	};

#undef DETAIL_TYPELISTENDNODE
#undef DETAIL_TYPELIST_PARAMS_USAGE
#undef DETAIL_TYPELIST_PARAMS_DECL


	//////////////////////////////////////////////////////////////////////


	namespace Detail
	{
		template < typename FirstTypeList, typename SecondTypeList >
		struct ListMerge
		{ typedef TypeListNode<typename FirstTypeList::ValueT, typename ListMerge<typename FirstTypeList::Next, SecondTypeList>::ValueT> ValueT; };

		template < typename SecondTypeList >
		struct ListMerge<TypeListEndNode, SecondTypeList>
		{ typedef SecondTypeList ValueT; };
	}

	template < typename TypeListOfTypeLists, typename ResultTypeList = TypeListEndNode >
	struct TypeListMerge
	{ typedef typename TypeListMerge<typename TypeListOfTypeLists::Next, typename Detail::ListMerge<ResultTypeList, typename TypeListOfTypeLists::ValueT>::ValueT>::ValueT ValueT; };

	template < typename ResultTypeList >
	struct TypeListMerge<TypeListEndNode, ResultTypeList>
	{ typedef ResultTypeList ValueT; };

	template < typename TypeList >
	struct GetTypeListLength
	{ static const size_t Value = GetTypeListLength<typename TypeList::Next>::Value + 1; };

	template < >
	struct GetTypeListLength<TypeListEndNode>
	{ static const size_t Value = 0; };

	template < typename TypeList, size_t N >
	struct GetTypeListItem
	{ typedef typename GetTypeListItem<typename TypeList::Next, N - 1>::ValueT	ValueT; };

	template < typename TypeList >
	struct GetTypeListItem<TypeList, 0>
	{ typedef typename TypeList::ValueT		ValueT; };

	template < typename TypeList >
	struct GetTypeListItem<TypeList, -1>
	{ CompileTimeAssert<sizeof(TypeList) < 0>	ERROR_invalid_index; };

	template < typename TypeList, typename Val >
	struct TypeListAppend
	{ typedef TypeListNode<typename TypeList::ValueT, typename TypeListAppend<typename TypeList::Next, Val>::ValueT>	ValueT; };

	template < typename Val >
	struct TypeListAppend<TypeListEndNode, Val>
	{ typedef TypeListNode<Val, TypeListEndNode>	ValueT; };

	template < typename TypeList, typename Val >
	struct TypeListPrepend
	{ typedef TypeListNode<Val, TypeList>	ValueT; };

	template < typename Val >
	struct TypeListPrepend<TypeListEndNode, Val>
	{ typedef TypeListNode<Val, TypeListEndNode>	ValueT; };

	template < typename TypeList, size_t N >
	struct TryGetTypeListItem
	{ typedef typename TryGetTypeListItem<typename TypeList::Next, N - 1>::ValueT	ValueT; };

	template < size_t N >
	struct TryGetTypeListItem<TypeListEndNode, N>
	{ typedef TypeListEndNode				ValueT; };

	template < typename TypeList >
	struct TryGetTypeListItem<TypeList, 0>
	{ typedef typename TypeList::ValueT		ValueT; };

	template < >
	struct TryGetTypeListItem<TypeListEndNode, 0>
	{ typedef TypeListEndNode				ValueT; };

	template < typename TypeList >
	struct TryGetTypeListItem<TypeList, -1>
	{ CompileTimeAssert<sizeof(TypeList) < 0>	ERROR_invalid_index; };

	template < typename TypeList, typename T, size_t Index_ = 0>
	struct IndexOfTypeListItem
	{
	private:
		static const int NextResult = IndexOfTypeListItem<typename TypeList::Next, T, Index_>::Value;
	public:
		static const int Value = (NextResult == -1) ? -1 : (NextResult + 1);
	};

	template < typename TypeList, typename T >
	struct TypeListContains : integral_constant<bool, IndexOfTypeListItem<TypeList, T>::Value != -1> { };

	template < typename T, size_t Index_ >
	struct IndexOfTypeListItem<TypeListEndNode, T, Index_> : integral_constant<int, -1> { };

	template < typename TypeList, size_t Index_ >
	struct IndexOfTypeListItem<TypeList, typename TypeList::ValueT, Index_>
	{
	private:
		static const int NextResult = IndexOfTypeListItem<typename TypeList::Next, typename TypeList::ValueT, Index_ - 1>::Value;
	public:
		static const int Value = (NextResult == -1) ? -1 : (NextResult + 1);
	};

	template < typename TypeList >
	struct IndexOfTypeListItem<TypeList, typename TypeList::ValueT, 0> : integral_constant<int, 0> { };


	template < typename TypeList, template<typename> class Predicate >
	struct EvaluateTypeListItem : integral_constant<bool, Predicate<typename TypeList::ValueT>::Value> { };

	template < template<typename> class Predicate >
	struct EvaluateTypeListItem<TypeListEndNode, Predicate> : FalseType { };

	template
		<
			typename TypeList,
			template <typename> class Predicate,
			bool CurrentIsOK = EvaluateTypeListItem<TypeList, Predicate>::Value
		>
	struct TypeListCopyIf;


	template < template <typename> class Predicate >
	struct TypeListCopyIf<TypeListEndNode, Predicate, true>		{ typedef TypeListEndNode	ValueT; };
	template < template <typename> class Predicate >
	struct TypeListCopyIf<TypeListEndNode, Predicate, false>	{ typedef TypeListEndNode	ValueT; };


	template < typename TypeList, template <typename> class Predicate >
	struct TypeListCopyIf<TypeList, Predicate, true>
	{ typedef TypeListNode<typename TypeList::ValueT, typename TypeListCopyIf<typename TypeList::Next, Predicate>::ValueT> ValueT; };

	template < typename TypeList, template <typename> class Predicate >
	struct TypeListCopyIf<TypeList, Predicate, false>
	{ typedef typename TypeListCopyIf<typename TypeList::Next, Predicate>::ValueT ValueT; };


	template < typename TypeList, template <typename, typename> class AccumulateFunc, typename InitialValue >
	struct TypeListAccumulate
	{ typedef typename AccumulateFunc<typename TypeList::ValueT, typename TypeListAccumulate<typename TypeList::Next, AccumulateFunc, InitialValue>::ValueT >::ValueT	ValueT; };

	template < template <typename, typename> class AccumulateFunc, typename InitialValue >
	struct TypeListAccumulate<TypeListEndNode, AccumulateFunc, InitialValue>
	{ typedef InitialValue	ValueT; };


	template < typename TypeList, template <typename> class Predicate >
	struct TypeListAllOf : integral_constant<bool, Predicate<typename TypeList::ValueT>::Value ? TypeListAllOf<typename TypeList::Next, Predicate>::Value : false> { };

	template < template <typename> class Predicate >
	struct TypeListAllOf<TypeListEndNode, Predicate> : TrueType { };


	template < typename TypeList, template <typename> class Predicate >
	struct TypeListAnyOf : integral_constant<bool, Predicate<typename TypeList::ValueT>::Value ? true : TypeListAnyOf<typename TypeList::Next, Predicate>::Value> { };

	template < template <typename> class Predicate >
	struct TypeListAnyOf<TypeListEndNode, Predicate> : FalseType { };


	template < typename TypeList, template <typename> class TransformFunc >
	struct TypeListTransform
	{ typedef TypeListNode<typename TransformFunc<typename TypeList::ValueT>::ValueT, typename TypeListTransform<typename TypeList::Next, TransformFunc>::ValueT> ValueT; };

	template < template <typename> class TransformFunc >
	struct TypeListTransform<TypeListEndNode, TransformFunc>
	{ typedef TypeListEndNode ValueT; };


	template < typename TypeList, template <typename> class Predicate >
	struct TypeListFindIf
	{ typedef typename If<Predicate<typename TypeList::ValueT>::Value, typename TypeList::ValueT, typename TypeListFindIf<typename TypeList::Next, Predicate>::ValueT>::ValueT ValueT; };

	template < template <typename> class Predicate >
	struct TypeListFindIf<TypeListEndNode, Predicate>
	{ typedef TypeListEndNode ValueT; };


	template < typename TypeList >
	struct TypeListReverse
	{ typedef typename TypeListAppend<typename TypeListReverse<typename TypeList::Next>::ValueT, typename TypeList::ValueT>::ValueT ValueT;};

	template < >
	struct TypeListReverse<TypeListEndNode>
	{ typedef TypeListEndNode ValueT; };


	template < unsigned Count, template <int> class GeneratorFunctor, int Start = 0 >
	struct GenerateTypeList
	{ typedef TypeListNode<typename GeneratorFunctor<Start>::ValueT, typename GenerateTypeList<Count - 1, GeneratorFunctor, Start + 1>::ValueT> ValueT; };

	template < template <int> class GeneratorFunctor, int Start >
	struct GenerateTypeList<0, GeneratorFunctor, Start>
	{ typedef TypeListEndNode ValueT; };


	template<typename TypeList, template<typename, typename> class LessPredicate, typename Tail = typename TypeList::Next>
	struct MaxElement
	{
	private:
		typedef typename MaxElement<typename TypeList::Next, LessPredicate>::ValueT NextResult;

	public:
		typedef typename If<LessPredicate<typename TypeList::ValueT, NextResult>::Value, NextResult, typename TypeList::ValueT>::ValueT ValueT;
	};

	template<typename TypeList, template<typename, typename> class LessPredicate>
	struct MaxElement<TypeList, LessPredicate, TypeListEndNode>
	{ typedef typename TypeList::ValueT ValueT; };


	template<typename TypeList, template<typename, typename> class LessPredicate, typename Tail = typename TypeList::Next>
	struct MinElement
	{
	private:
		typedef typename MinElement<typename TypeList::Next, LessPredicate>::ValueT NextResult;

	public:
		typedef typename If<LessPredicate<typename TypeList::ValueT, NextResult>::Value, typename TypeList::ValueT, NextResult>::ValueT ValueT;
	};

	template<typename TypeList, template<typename, typename> class LessPredicate>
	struct MinElement<TypeList, LessPredicate, TypeListEndNode>
	{ typedef typename TypeList::ValueT ValueT; };


	template <typename TypeList_, template<typename, typename> class LessPredicate>
	struct TypeListSort
	{
		typedef typename TypeListMerge<TypeList<
				typename TypeListSort<typename TypeListCopyIf<typename TypeList_::Next, BindRight<LessPredicate, typename TypeList_::ValueT>::template ValueT>::ValueT, LessPredicate>::ValueT,
				TypeList<typename TypeList_::ValueT>,
				typename TypeListSort<typename TypeListCopyIf<typename TypeList_::Next, Not<BindRight<LessPredicate, typename TypeList_::ValueT>::template ValueT>::template ValueT>::ValueT, LessPredicate>::ValueT> >::ValueT ValueT;
	};

	template <template<typename, typename> class LessPredicate>
	struct TypeListSort<TypeListEndNode, LessPredicate>
	{ typedef TypeListEndNode ValueT; };


	namespace Detail
	{

		template < typename SortedTypeList, template <typename, typename, typename> class BranchType, typename LeafType, size_t Begin = 0, size_t End = GetTypeListLength<SortedTypeList>::Value >
		struct BalancedTypeTreeImpl
		{
		private:
			static const size_t Index = Begin + (End - Begin) / 2;

		public:
			typedef BranchType<
					typename GetTypeListItem<SortedTypeList, Index>::ValueT,
					typename BalancedTypeTreeImpl<SortedTypeList, BranchType, LeafType, Begin, Index>::ValueT,
					typename BalancedTypeTreeImpl<SortedTypeList, BranchType, LeafType, Index + 1, End>::ValueT
			> ValueT;
		};

		template < typename SortedTypeList, template <typename, typename, typename> class BranchType, typename LeafType, size_t Begin >
		struct BalancedTypeTreeImpl<SortedTypeList, BranchType, LeafType, Begin, Begin>
		{ typedef LeafType ValueT; };

	}

	template < typename TypeList, template <typename, typename> class LessPredicate, template <typename, typename, typename> class BranchType, typename LeafType >
	struct BalancedTypeTree
	{ typedef typename Detail::BalancedTypeTreeImpl<typename TypeListSort<TypeList, LessPredicate>::ValueT, BranchType, LeafType>::ValueT ValueT; };


	template < typename TypeList, template <typename> class FunctorClass >
	struct ForEachInTypeList
	{
		static void Do()
		{
			FunctorClass<typename TypeList::ValueT>::Call();
			ForEachInTypeList<typename TypeList::Next, FunctorClass>::Do();
		}

#define DETAIL_STINGRAYKIT_DECLARE_FOREACHINTYPELIST_DO(TypesDecl_, TypesUsage_, ParamsDecl_, ParamsUsage_) \
		template < TypesDecl_ > \
		static void Do(ParamsDecl_) \
		{ \
			FunctorClass<typename TypeList::ValueT>::Call(ParamsUsage_); \
			ForEachInTypeList<typename TypeList::Next, FunctorClass>::template Do<TypesUsage_>(ParamsUsage_); \
		}

		DETAIL_STINGRAYKIT_DECLARE_FOREACHINTYPELIST_DO(MK_PARAM(TY T1), MK_PARAM(T1), MK_PARAM(const T1& p1), MK_PARAM(p1));
		DETAIL_STINGRAYKIT_DECLARE_FOREACHINTYPELIST_DO(MK_PARAM(TY T1, TY T2), MK_PARAM(T1, T2), MK_PARAM(const T1& p1, const T2& p2), MK_PARAM(p1, p2));
		DETAIL_STINGRAYKIT_DECLARE_FOREACHINTYPELIST_DO(MK_PARAM(TY T1, TY T2, TY T3), MK_PARAM(T1, T2, T3), MK_PARAM(const T1& p1, const T2& p2, const T3& p3), MK_PARAM(p1, p2, p3));
		DETAIL_STINGRAYKIT_DECLARE_FOREACHINTYPELIST_DO(MK_PARAM(TY T1, TY T2, TY T3, TY T4), MK_PARAM(T1, T2, T3, T4), MK_PARAM(const T1& p1, const T2& p2, const T3& p3, const T4& p4), MK_PARAM(p1, p2, p3, p4));
		DETAIL_STINGRAYKIT_DECLARE_FOREACHINTYPELIST_DO(MK_PARAM(TY T1, TY T2, TY T3, TY T4, TY T5), MK_PARAM(T1, T2, T3, T4, T5), MK_PARAM(const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5), MK_PARAM(p1, p2, p3, p4, p5));

#undef DETAIL_STINGRAYKIT_DECLARE_FOREACHINTYPELIST_DO
	};

	template < template <typename> class FunctorClass >
	struct ForEachInTypeList<TypeListEndNode, FunctorClass>
	{
		static void Do() { }

		template <TY T1> static void Do(const T1&) { }
		template <TY T1, TY T2> static void Do(const T1&, const T2&) { }
		template <TY T1, TY T2, TY T3> static void Do(const T1&, const T2&, const T3&) { }
		template <TY T1, TY T2, TY T3, TY T4> static void Do(const T1&, const T2&, const T3&, const T4&) { }
		template <TY T1, TY T2, TY T3, TY T4, TY T5> static void Do(const T1&, const T2&, const T3&, const T4&, const T5&) { }
	};



	namespace Detail
	{

		template < typename T, bool IsTypeList >
		struct ToTypeListImpl { typedef T	ValueT; };

		template < typename T >
		struct ToTypeListImpl<T, false> { typedef TypeList<T> ValueT; };

	}

	template < typename T >
	class ToTypeList
	{
	private:
		template < typename V, typename N>
		static YesType TestTypeList(const TypeListNode<V, N>*);
		static YesType TestTypeList(const TypeListEndNode*);
		static NoType TestTypeList(...);

	public:
		typedef typename Detail::ToTypeListImpl<T, sizeof(YesType) == sizeof(TestTypeList((const T*)0))>::ValueT		ValueT;
	};

#undef TY

}


#endif
