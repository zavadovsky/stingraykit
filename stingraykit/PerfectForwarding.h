#ifndef STINGRAYKIT_PERFECTFORWARDING_H
#define STINGRAYKIT_PERFECTFORWARDING_H

// Copyright (c) 2011 - 2019, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.


#include <stingraykit/Macro.h>

namespace stingray
{

#define DETAIL_CONST_FORWARDING(N_, RetType_, FromFunction_, ToFunction_) \
	STINGRAYKIT_INSERT_IF(N_, template<STINGRAYKIT_REPEAT_NESTING_2(N_, STINGRAYKIT_TEMPLATE_PARAM_DECL, T) >) \
	RetType_ FromFunction_ (STINGRAYKIT_REPEAT_NESTING_2(N_, STINGRAYKIT_FUNCTION_PARAM_DECL, T)) const \
	{ \
		typedef STINGRAYKIT_INSERT_IF(N_, typename) TypeList<STINGRAYKIT_REPEAT_NESTING_2(N_, STINGRAYKIT_TEMPLATE_PARAM_USAGE, T)>::type PT; \
		Tuple<PT> p STINGRAYKIT_LEFT_BRACKET_IF(N_) STINGRAYKIT_REPEAT_NESTING_2(N_, STINGRAYKIT_FUNCTION_PARAM_USAGE, T) STINGRAYKIT_RIGHT_BRACKET_IF(N_); \
		return ToFunction_<PT>(p); \
	}

#define STINGRAYKIT_CONST_FORWARDING(RetType_, FromFunction_, ToFunction_) \
	STINGRAYKIT_REPEAT(9, DETAIL_CONST_FORWARDING, RetType_, FromFunction_, ToFunction_)


#define DETAIL_PERFECT_FORWARDING_OP_PARAM_TYPE(ParamIndex_, ParamVariantId_) \
	STINGRAYKIT_COMMA_IF(ParamIndex_) STINGRAYKIT_INSERT_IF(STINGRAYKIT_BITWISE_AND(ParamVariantId_, STINGRAYKIT_POW(2, ParamIndex_)), const) STINGRAYKIT_CAT(T, ParamIndex_)&

#define DETAIL_PERFECT_FORWARDING_OP_PARAM_DECL(ParamIndex_, ParamVariantId_) \
	DETAIL_PERFECT_FORWARDING_OP_PARAM_TYPE(ParamIndex_, ParamVariantId_) STINGRAYKIT_CAT(p, ParamIndex_)

#define DETAIL_PERFECT_FORWARDING(N_, ParamsCount_, RetType_, FromFunction_, ToFunction_) \
	template< STINGRAYKIT_REPEAT_NESTING_2(ParamsCount_, STINGRAYKIT_TEMPLATE_PARAM_DECL, T) > \
	RetType_ FromFunction_ (STINGRAYKIT_REPEAT_NESTING_2(ParamsCount_, DETAIL_PERFECT_FORWARDING_OP_PARAM_DECL, N_)) const \
	{ \
		typedef typename TypeList<STINGRAYKIT_REPEAT_NESTING_2(ParamsCount_, DETAIL_PERFECT_FORWARDING_OP_PARAM_TYPE, N_)>::type PT; \
		Tuple<PT> p(STINGRAYKIT_REPEAT_NESTING_2(ParamsCount_, STINGRAYKIT_FUNCTION_PARAM_USAGE, ~)); \
		return ToFunction_<PT>(p); \
	}

#define STINGRAYKIT_PERFECT_FORWARDING(RetType_, FromFunction_, ToFunction_) \
	RetType_ FromFunction_ () const \
	{ \
		typedef TypeList<>::type PT; \
		Tuple<PT> p; \
		return ToFunction_<PT>(p); \
	} \
	STINGRAYKIT_REPEAT( 2, DETAIL_PERFECT_FORWARDING, 1, RetType_, FromFunction_, ToFunction_) \
	STINGRAYKIT_REPEAT( 4, DETAIL_PERFECT_FORWARDING, 2, RetType_, FromFunction_, ToFunction_) \
	STINGRAYKIT_REPEAT( 8, DETAIL_PERFECT_FORWARDING, 3, RetType_, FromFunction_, ToFunction_) \
	STINGRAYKIT_REPEAT(16, DETAIL_PERFECT_FORWARDING, 4, RetType_, FromFunction_, ToFunction_) \
	STINGRAYKIT_REPEAT(32, DETAIL_PERFECT_FORWARDING, 5, RetType_, FromFunction_, ToFunction_) \
	STINGRAYKIT_REPEAT(64, DETAIL_PERFECT_FORWARDING, 6, RetType_, FromFunction_, ToFunction_)

}

#endif
