#ifndef STINGRAYKIT_FUNCTION_CANCELLABLEFUNCTION_H
#define STINGRAYKIT_FUNCTION_CANCELLABLEFUNCTION_H

namespace stingray
{

	template< typename FunctorType >
	class CancellableFunction : public function_info<FunctorType>
	{
	private:
		FunctorType				_func;
		FutureExecutionTester	_tester;

	public:
		CancellableFunction(const FunctorType& func, const FutureExecutionTester& tester) :
			_func(func), _tester(tester)
		{ }

		STINGRAYKIT_PERFECT_FORWARDING(void, operator (), Do)

	private:
		template< typename ParamTypeList >
		void Do(const Tuple<ParamTypeList>& params) const
		{
			LocalExecutionGuard guard;
			if (_tester.Execute(guard))
				FunctorInvoker::Invoke(_func, params);
		}
	};


	template < typename FuncType >
	CancellableFunction<FuncType> MakeCancellableFunction(const FuncType& func, const FutureExecutionTester& tester)
	{ return CancellableFunction<FuncType>(func, tester); }

}

#endif