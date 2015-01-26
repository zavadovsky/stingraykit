#ifndef STINGRAYKIT_THREAD_POSIX_POSIXSEMAPHORE_H
#define STINGRAYKIT_THREAD_POSIX_POSIXSEMAPHORE_H

#include <pthread.h>
#include <semaphore.h>

#include <stingraykit/time/Time.h>
#include <stingraykit/exception.h>

namespace stingray {
namespace posix {


	class PosixSemaphore
	{
		STINGRAYKIT_NONCOPYABLE(PosixSemaphore);

	private:
		mutable sem_t	_semaphore;

	public:
		PosixSemaphore(int count = 0);
		~PosixSemaphore();

		void Signal() const;
		void Signal(int n) const { while(n-- > 0) Signal(); }

		void Wait() const;

		bool TimedWait(TimeDuration interval) const;
		bool TimedWait(const Time& absTime) const;
	};


}
}


#endif
