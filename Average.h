#ifndef STINGRAYKIT_AVERAGE_H
#define STINGRAYKIT_AVERAGE_H


namespace stingray 
{


	template < typename T, typename CounterT = s32 >
	class Average
	{
	private:
		T			_accum;
		CounterT	_count;

	public:
		Average() : _accum(), _count() { }

		void Accumulate(T val)	{ _accum += val; ++_count; }
		T Get() const			{ return _count > 0 ? _accum / _count : T(); }
	};


}


#endif
