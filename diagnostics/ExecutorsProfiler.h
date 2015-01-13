#ifndef STINGRAYKIT_DIAGNOSTICS_EXECUTORSPROFILER_H
#define STINGRAYKIT_DIAGNOSTICS_EXECUTORSPROFILER_H


#include <stingray/toolkit/diagnostics/AsyncProfiler.h>
#include <stingray/toolkit/PhoenixSingleton.h>


namespace stingray
{

	/**
	 * @addtogroup toolkit_profiling
	 * @{
	 */

	class ExecutorsProfiler : public PhoenixSingleton<ExecutorsProfiler>
	{
		STINGRAYKIT_PHOENIXSINGLETON(ExecutorsProfiler);

	private:
		AsyncProfilerPtr	_profiler;

	private:
		ExecutorsProfiler()
			: _profiler(new AsyncProfiler("executorsProfiler"))
		{ }

	public:
		AsyncProfilerWeakPtr GetProfiler() const { return _profiler; }
	};

	/** @} */

}

#endif
