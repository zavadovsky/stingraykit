#include <dvrlib/toolkit/ITaskExecutor.h>

#if defined(PLATFORM_OSPLUS)
#	include <dvrlib/platform/osplus/threads/OS21TaskExecutor.h>
#else
#	include <dvrlib/toolkit/ThreadTaskExecutor.h>
#endif
#include <dvrlib/log/Logger.h>


namespace dvrlib
{

	ITaskExecutorPtr ITaskExecutor::Create(const std::string& name, const function<void(const std::exception&)>& exceptionHandler) 
#if defined(PLATFORM_OSPLUS)
	{ return make_shared<osplus::TaskExecutor>(name, exceptionHandler); }
#else
	{ return make_shared<ThreadTaskExecutor>(name, exceptionHandler); }
#endif


	void ITaskExecutor::DefaultExceptionHandler(const std::exception& ex)
	{ Logger::Error() << "Uncaught exception in ITaskExecutor: " << diagnostic_information(ex); }

}
