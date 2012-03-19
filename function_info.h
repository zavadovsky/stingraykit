#ifndef __GS_DVRLIB_TOOLKIT_FUNCTION_INFO_H__
#define __GS_DVRLIB_TOOLKIT_FUNCTION_INFO_H__


#include <dvrlib/toolkit/Macro.h>
#include <dvrlib/toolkit/toolkit.h>
#include <dvrlib/toolkit/TypeList.h>

/*! \cond GS_INTERNAL */

namespace dvrlib
{

	struct FunctionType
	{
		TOOLKIT_ENUM_VALUES
		(
			RawFunction, 
			RawFunctionPtr,
			MethodPtr,
			GSFunction, 
			StdFunction, 
			Other
		);

		TOOLKIT_DECLARE_ENUM_CLASS(FunctionType);
	};


#define TY typename
	
	template < typename F >
	struct function_info;
	template < typename F >
	struct function_type;

/////////////////////////////////////
/// for raw functions and methods ///
/////////////////////////////////////

	template < typename R > struct function_type<R()>
	{ static const FunctionType::Enum Type = FunctionType::RawFunction; };
	template < typename R > struct function_type<R(*)()>
	{ static const FunctionType::Enum Type = FunctionType::RawFunctionPtr; };
	template < typename C, typename R > struct function_type<R (C::*)()>
	{ static const FunctionType::Enum Type = FunctionType::MethodPtr; };
	template < typename C, typename R > struct function_type<R (C::*)() const>
	{ static const FunctionType::Enum Type = FunctionType::MethodPtr; };
	template < typename R >
	struct function_info<R()> : public function_type<R()>
	{
		typedef R			RetType;
		typedef TypeList_0	ParamTypes;
	};
	template < typename R >
	struct function_info<R(*)()> : public function_type<R(*)()>
	{
		typedef R			RetType;
		typedef TypeList_0	ParamTypes;
	};
	template < typename C, typename R >
	struct function_info<R (C::*)()> : public function_type<R (C::*)()>
	{
		typedef R				RetType;
		typedef TypeList_1<C*>	ParamTypes;
	};
	template < typename C, typename R >
	struct function_info<R (C::*)() const> : public function_type<R (C::*)() const>
	{
		typedef R						RetType;
		typedef TypeList_1<const C*>	ParamTypes;
	};


#define DETAIL_TOOLKIT_DECLARE_FUNCTION_INFO(ParamTypenames_, ParamTypes_) \
	template < typename R, ParamTypenames_ > struct function_type<R(ParamTypes_)> \
	{ static const FunctionType::Enum Type = FunctionType::RawFunction; }; \
	template < typename R, ParamTypenames_ > struct function_type<R(*)(ParamTypes_)> \
	{ static const FunctionType::Enum Type = FunctionType::RawFunctionPtr; }; \
	template < typename C, typename R, ParamTypenames_ > struct function_type<R (C::*)(ParamTypes_)> \
	{ static const FunctionType::Enum Type = FunctionType::MethodPtr; }; \
	template < typename C, typename R, ParamTypenames_ > struct function_type<R (C::*)(ParamTypes_) const> \
	{ static const FunctionType::Enum Type = FunctionType::MethodPtr; }; \
	template < typename R, ParamTypenames_ > \
	struct function_info<R(ParamTypes_)> : function_type<R(ParamTypes_)> \
	{ \
		typedef R								RetType; \
		typedef TYPELIST(ParamTypes_)			ParamTypes; \
	}; \
	template < typename R, ParamTypenames_ > \
	struct function_info<R(*)(ParamTypes_)> : function_type<R(*)(ParamTypes_)> \
	{ \
		typedef R								RetType; \
		typedef TYPELIST(ParamTypes_)			ParamTypes; \
	}; \
	template < typename C, typename R, ParamTypenames_ > \
	struct function_info<R (C::*)(ParamTypes_)> : function_type<R (C::*)(ParamTypes_)> \
	{ \
		typedef R								RetType; \
		typedef TYPELIST(C*, ParamTypes_)		ParamTypes; \
	}; \
	template < typename C, typename R, ParamTypenames_ > \
	struct function_info<R (C::*)(ParamTypes_) const>  : function_type<R (C::*)(ParamTypes_) const> \
	{ \
		typedef R								RetType; \
		typedef TYPELIST(const C*, ParamTypes_)	ParamTypes; \
	}

	DETAIL_TOOLKIT_DECLARE_FUNCTION_INFO(MK_PARAM(TY T1), MK_PARAM(T1));
	DETAIL_TOOLKIT_DECLARE_FUNCTION_INFO(MK_PARAM(TY T1, TY T2), MK_PARAM(T1, T2));
	DETAIL_TOOLKIT_DECLARE_FUNCTION_INFO(MK_PARAM(TY T1, TY T2, TY T3), MK_PARAM(T1, T2, T3));
	DETAIL_TOOLKIT_DECLARE_FUNCTION_INFO(MK_PARAM(TY T1, TY T2, TY T3, TY T4), MK_PARAM(T1, T2, T3, T4));
	DETAIL_TOOLKIT_DECLARE_FUNCTION_INFO(MK_PARAM(TY T1, TY T2, TY T3, TY T4, TY T5), MK_PARAM(T1, T2, T3, T4, T5));
	DETAIL_TOOLKIT_DECLARE_FUNCTION_INFO(MK_PARAM(TY T1, TY T2, TY T3, TY T4, TY T5, TY T6), MK_PARAM(T1, T2, T3, T4, T5, T6));
	DETAIL_TOOLKIT_DECLARE_FUNCTION_INFO(MK_PARAM(TY T1, TY T2, TY T3, TY T4, TY T5, TY T6, TY T7), MK_PARAM(T1, T2, T3, T4, T5, T6, T7));
	DETAIL_TOOLKIT_DECLARE_FUNCTION_INFO(MK_PARAM(TY T1, TY T2, TY T3, TY T4, TY T5, TY T6, TY T7, TY T8), MK_PARAM(T1, T2, T3, T4, T5, T6, T7, T8));
	DETAIL_TOOLKIT_DECLARE_FUNCTION_INFO(MK_PARAM(TY T1, TY T2, TY T3, TY T4, TY T5, TY T6, TY T7, TY T8, TY T9), MK_PARAM(T1, T2, T3, T4, T5, T6, T7, T8, T9));
	DETAIL_TOOLKIT_DECLARE_FUNCTION_INFO(MK_PARAM(TY T1, TY T2, TY T3, TY T4, TY T5, TY T6, TY T7, TY T8, TY T9, TY T10), MK_PARAM(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10));

#undef DETAIL_TOOLKIT_DECLARE_FUNCTION_INFO


/////////////////////////////
/// for toolkit::function ///
/////////////////////////////

	template < typename Signature >
	class function;

	template < typename Signature >
	struct function_type<function<Signature> >
	{ static const FunctionType::Enum Type = FunctionType::GSFunction; };


	template < typename Signature >
	struct function_info<function<Signature> > : public function_type<function<Signature> >
	{ 
		typedef typename function_info<Signature>::RetType		RetType;
		typedef typename function_info<Signature>::ParamTypes	ParamTypes;
	};


////////////////////////////////////////////////
/// assuming that all other are stl functors ///
////////////////////////////////////////////////

	namespace Detail
	{
		template < typename F >
		class GetStlFunctorNumArguments
		{
		private:
			struct _wtf_ { char dummy[2]; };
			struct _1_ { _wtf_ dummy[2]; };
			struct _2_ { _1_ dummy[2]; };

			template < typename R, typename T1, typename T2 >
			static _2_ Test(const std::binary_function<T1, T2, R>*);

			template < typename R, typename T1 >
			static _1_ Test(const std::unary_function<T1, R>*);
			
			static _wtf_ Test(...);
			CompileTimeAssert<sizeof(_1_) != sizeof(_2_) && sizeof(_2_) != sizeof(_wtf_) && sizeof(_1_) != sizeof(_wtf_)> ERROR__type_detectors_dont_work;
		public:
			static const int Value = 
				(sizeof(Test((const F*)0)) == sizeof(_1_) 
					? 1 
					: (sizeof(Test((const F*)0)) == sizeof(_2_) 
						? 2
						: -1
					)
				);
		};

		template < typename F, int StlFunctorNumArguments = GetStlFunctorNumArguments<F>::Value >
		struct std_function_type
		{ static const FunctionType::Enum Type = FunctionType::Other; };

		template < typename F, int StlFunctorNumArguments = GetStlFunctorNumArguments<F>::Value >
		struct std_function_info : public std_function_type<F>
		{
			typedef typename F::RetType		RetType;
			typedef typename F::ParamTypes	ParamTypes;
		};

		template < typename F> struct std_function_type<F, 1>
		{ static const FunctionType::Enum Type = FunctionType::StdFunction; };

		template < typename F >
		struct std_function_info<F, 1> : public std_function_type<F>
		{
			typedef typename F::result_type						RetType;
			typedef TYPELIST(typename F::argument_type)			ParamTypes;
		};

		template < typename F> struct std_function_type<F, 2>
		{ static const FunctionType::Enum Type = FunctionType::StdFunction; };

		template < typename F >
		struct std_function_info<F, 2> : public std_function_type<F>
		{
			typedef typename F::result_type						RetType;
			typedef TYPELIST(typename F::first_argument_type, 
							 typename F::second_argument_type)	ParamTypes;
		};
	}

	template < typename F >
	struct function_type : public Detail::std_function_type<F>
	{ };

	template < typename F >
	struct function_info : public Detail::std_function_info<F>
	{ };


#undef TY

}

/*! \endcond */


#endif
