#ifndef STINGRAY_TOOLKIT_RATIONAL_H
#define STINGRAY_TOOLKIT_RATIONAL_H

namespace stingray
{
	struct Rational
	{
		int Num, Denum;
		Rational(int Num = 0, int Denum = 1): Num(Num), Denum(Denum) {}
	};
}

#endif	/* RATIONAL_H */

