#ifndef __GS_DVRLIB_TOOLKIT_PROGRESSVALUE_H__
#define __GS_DVRLIB_TOOLKIT_PROGRESSVALUE_H__


#include <dvrlib/toolkit/StringUtils.h>


namespace dvrlib
{


	struct ProgressValue
	{
		int		Current;
		int		Total;

		ProgressValue(int current, int total)
			: Current(current), Total(total)
		{ }

		size_t InPercents() const
		{ return 100.0 * Current / Total; }

		std::string ToString() const
		{ return StringBuilder() % InPercents() % "%"; }
	};


}


#endif
