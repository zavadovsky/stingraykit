#ifndef STINGRAYKIT_THREAD_THREADOPERATION_H
#define STINGRAYKIT_THREAD_THREADOPERATION_H


#include <stingray/toolkit/log/Logger.h>
#include <stingray/toolkit/toolkit.h>


namespace stingray
{

	struct ThreadOperation
	{
		STINGRAYKIT_ENUM_VALUES(Network, IO);
		STINGRAYKIT_DECLARE_ENUM_CLASS(ThreadOperation);
	};


	class ThreadOperationReporter
	{
	private:
		static NamedLogger	s_logger;

	public:
		ThreadOperationReporter(ThreadOperation op);
		~ThreadOperationReporter();
	};

}

#endif
