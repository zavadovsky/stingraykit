#ifndef STINGRAYKIT_REF_COUNT_H
#define STINGRAYKIT_REF_COUNT_H


#include <assert.h>
#include <stingray/toolkit/Atomic.h>
#include <stingray/toolkit/toolkit.h>



namespace stingray
{

	template < typename UserDataType >
	class basic_ref_count_data
	{
		STINGRAYKIT_NONCOPYABLE(basic_ref_count_data);

	public:
		atomic_int_type		Value;
		atomic_int_type		SelfCount;
		UserDataType		UserData;

		inline basic_ref_count_data(const UserDataType& userData)
			: Value(1), SelfCount(1), UserData(userData)
		{ }
	};

	template < >
	class basic_ref_count_data<NullType>
	{
		STINGRAYKIT_NONCOPYABLE(basic_ref_count_data);

	public:
		atomic_int_type		Value;
		atomic_int_type		SelfCount;

		inline basic_ref_count_data()
			: Value(1), SelfCount(1)
		{ }

		inline basic_ref_count_data(const NullType& userData)
			: Value(1), SelfCount(1)
		{ }
	};


	template < typename UserDataType >
	class basic_ref_count
	{
		typedef basic_ref_count_data<UserDataType>		Data;

		Data		*_value;

	public:
		inline basic_ref_count(const NullPtrType&) : _value()
		{ }

		inline basic_ref_count() : _value(new Data)
		{ }

		inline basic_ref_count(const UserDataType& userData) :
			_value(new Data(userData))
		{ }

		inline basic_ref_count(const basic_ref_count& other) :
			_value(other._value)
		{ if (_value) add_ref_self(); }

		inline ~basic_ref_count()
		{ if (_value) release_self(); }

		inline basic_ref_count& operator = (const basic_ref_count& other)
		{
			basic_ref_count tmp(other);
			swap(tmp);
			return *this;
		}

		inline const UserDataType& GetUserData() const { assert(_value); return _value->UserData; }
		inline bool IsNull() const { return !_value; }

		inline atomic_int_type get() const	{ assert(_value); return _value->Value; }
		inline atomic_int_type add_ref()	{ assert(_value); return Atomic::Inc(_value->Value); }
		inline atomic_int_type release()	{ assert(_value); return Atomic::Dec(_value->Value); }
		inline bool release_if_unique()		{ assert(_value); return Atomic::CompareAndExchange(_value->Value, 1, 0) == 1; }

		inline void swap(basic_ref_count& other)
		{ std::swap(_value, other._value); }

		const Data* get_ptr() const
		{ return _value; }

	private:
		inline atomic_int_type add_ref_self()
		{
			assert(_value);
			atomic_int_type result = Atomic::Inc(_value->SelfCount);
			assert(result > 0);
			return result;
		}
		inline atomic_int_type release_self()
		{
			assert(_value);
			atomic_int_type result = Atomic::Dec(_value->SelfCount);
			assert(result >= 0);
			if (result == 0)
			{
				STINGRAYKIT_ANNOTATE_HAPPENS_AFTER(_value);
				STINGRAYKIT_ANNOTATE_RELEASE(_value);
				delete _value;
			}
			else
				STINGRAYKIT_ANNOTATE_HAPPENS_BEFORE(_value);
			return result;
		}
	};


	typedef basic_ref_count<NullType>		ref_count;


}



#endif
