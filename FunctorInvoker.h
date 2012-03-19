#ifndef __GS_DVRLIB_TOOLKIT_FUNCTORINVOKER_H__
#define __GS_DVRLIB_TOOLKIT_FUNCTORINVOKER_H__


#include <dvrlib/toolkit/function_info.h>
#include <dvrlib/toolkit/exception.h>


/*! \cond GS_INTERNAL */

namespace dvrlib
{

	template < typename T >
	struct ToPointerType<T*>
	{ typedef T* ValueT; };

	template < typename T > T* to_pointer(T* ptr) { return ptr; }


	namespace Detail
	{

		template < size_t ParametersCount, bool IsMethod >
		struct FunctorInvokerImpl;

		template < > struct FunctorInvokerImpl<0, false>
		{
			template < typename FunctorType, typename ParametersTuple >
			static FORCE_INLINE typename function_info<FunctorType>::RetType Invoke(const FunctorType& func, const ParametersTuple& p)
			{ 
				//CompileTimeAssert<ParametersTuple::Size == 0> ERROR__invalid_number_of_parameters;
				return func(); 
			}
		};

#define DETAIL_TOOLKIT_DECLARE_FUNCTOR_INVOKER_IMPL(N, ...) \
		template < > struct FunctorInvokerImpl<N, false> \
		{ \
			template < typename FunctorType, typename ParametersTuple > \
			static FORCE_INLINE typename function_info<FunctorType>::RetType Invoke(const FunctorType& func, const ParametersTuple& p) \
			{ \
				/*CompileTimeAssert<ParametersTuple::Size == 0> ERROR__invalid_number_of_parameters;*/ \
				return func(p.template Get<0>(), ##__VA_ARGS__);  \
			} \
		}; \
		template < > struct FunctorInvokerImpl<N, true> \
		{ \
			template < typename FunctorType, typename ParametersTuple > \
			static FORCE_INLINE typename function_info<FunctorType>::RetType Invoke(const FunctorType& func, const ParametersTuple& p) \
			{ \
				/*CompileTimeAssert<ParametersTuple::Size == 0> ERROR__invalid_number_of_parameters;*/ \
				return (TOOLKIT_REQUIRE_NOT_NULL(to_pointer(p.template Get<0>()))->*func)(__VA_ARGS__);  \
			} \
		}

#define P(N) p.template Get<N>()

		DETAIL_TOOLKIT_DECLARE_FUNCTOR_INVOKER_IMPL(1);
		DETAIL_TOOLKIT_DECLARE_FUNCTOR_INVOKER_IMPL(2, MK_PARAM(P(1)));
		DETAIL_TOOLKIT_DECLARE_FUNCTOR_INVOKER_IMPL(3, MK_PARAM(P(1), P(2)));
		DETAIL_TOOLKIT_DECLARE_FUNCTOR_INVOKER_IMPL(4, MK_PARAM(P(1), P(2), P(3)));
		DETAIL_TOOLKIT_DECLARE_FUNCTOR_INVOKER_IMPL(5, MK_PARAM(P(1), P(2), P(3), P(4)));
		DETAIL_TOOLKIT_DECLARE_FUNCTOR_INVOKER_IMPL(6, MK_PARAM(P(1), P(2), P(3), P(4), P(5)));
		DETAIL_TOOLKIT_DECLARE_FUNCTOR_INVOKER_IMPL(7, MK_PARAM(P(1), P(2), P(3), P(4), P(5), P(6)));
		DETAIL_TOOLKIT_DECLARE_FUNCTOR_INVOKER_IMPL(8, MK_PARAM(P(1), P(2), P(3), P(4), P(5), P(6), P(7)));
		DETAIL_TOOLKIT_DECLARE_FUNCTOR_INVOKER_IMPL(9, MK_PARAM(P(1), P(2), P(3), P(4), P(5), P(6), P(7), P(8)));
		DETAIL_TOOLKIT_DECLARE_FUNCTOR_INVOKER_IMPL(10, MK_PARAM(P(1), P(2), P(3), P(4), P(5), P(6), P(7), P(8), P(9)));

#undef P
		
	}


	struct FunctorInvoker
	{
		template < typename FunctorType, typename ParamsTuple >
		static FORCE_INLINE typename function_info<FunctorType>::RetType Invoke(const FunctorType& func, const ParamsTuple& p)
		{
			return Detail::FunctorInvokerImpl
				<
					ParamsTuple::Size, 
					/*GetTypeListLength<typename function_info<FunctorType>::ParamTypes>::Value,*/ 
					function_info<FunctorType>::Type == FunctionType::MethodPtr
				>::template Invoke<FunctorType, ParamsTuple>(func, p);
		}
	};


}

/*! \endcond */


#endif
