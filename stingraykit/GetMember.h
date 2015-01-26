#ifndef STINGRAYKIT_GETMEMBER_H
#define STINGRAYKIT_GETMEMBER_H


#include <stingraykit/function/function_info.h>


namespace stingray
{

	namespace Detail
	{

		template < typename C, typename T >
		class MemberGetter : public function_info<T(const C&)>
		{
			typedef T C::*MemberPointer;

		private:
			MemberPointer	_member;

		public:
			MemberGetter(MemberPointer member) : _member(member) { }

			T operator()(const C& c) const { return c.*_member; }
		};

	}

	template < typename C, typename T >
	Detail::MemberGetter<C, T> GetMember(T C::* const member) { return Detail::MemberGetter<C, T>(member); }

}


#endif