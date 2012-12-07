#ifndef STINGRAY_TOOLKIT_IBINARYBUFFER_H
#define STINGRAY_TOOLKIT_IBINARYBUFFER_H


#include <stingray/toolkit/shared_ptr.h>


namespace stingray
{

	struct IBinaryBuffer
	{
		virtual ~IBinaryBuffer() { }

		virtual size_t Read(size_t offset, void* data, size_t count) const = 0;
		virtual size_t Write(size_t offset, void* data, size_t count) = 0;

		virtual size_t GetLength() const = 0;
	};
	TOOLKIT_DECLARE_PTR(IBinaryBuffer);

}


#endif
