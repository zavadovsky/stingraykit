#ifndef STINGRAYKIT_IO_IPIPE_H
#define STINGRAYKIT_IO_IPIPE_H


#include <stingraykit/collection/ByteData.h>
#include <stingraykit/shared_ptr.h>
#include <stingraykit/thread/ICancellationToken.h>


namespace stingray
{

	struct IPipe
	{
		virtual ~IPipe() {}

		virtual size_t Read(ByteData dst, const ICancellationToken& token) = 0;
		virtual size_t Write(ConstByteData src, const ICancellationToken& token) = 0;
	};
	STINGRAYKIT_DECLARE_PTR(IPipe);

}


#endif
