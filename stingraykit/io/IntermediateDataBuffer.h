#ifndef STINGRAYKIT_IO_INTERMEDIATEDATABUFFER_H
#define STINGRAYKIT_IO_INTERMEDIATEDATABUFFER_H

// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/io/DataSources.h>

namespace stingray
{

	class IntermediateDataBuffer final : public virtual IDataSource
	{
	private:
		IDataSourcePtr			_source;
		IDataBufferPtr			_buffer;

		ThreadPtr				_worker;

	public:
		IntermediateDataBuffer(const std::string& threadName, const IDataSourcePtr& source, const IDataBufferPtr& buffer)
			:	_source(STINGRAYKIT_REQUIRE_NOT_NULL(source)),
				_buffer(STINGRAYKIT_REQUIRE_NOT_NULL(buffer)),
				_worker(make_shared_ptr<Thread>(threadName, Bind(&IntermediateDataBuffer::ThreadFunc, this, _1)))
		{ }

		void Read(IDataConsumer& consumer, const ICancellationToken& token) override
		{ _buffer->Read(consumer, token); }

	private:
		void ThreadFunc(const ICancellationToken& token)
		{
			try
			{ ReactiveDataSource(_source).Read(*_buffer, token); }
			catch (const std::exception& ex)
			{ _buffer->SetException(ex, token); }
		}
	};
	STINGRAYKIT_DECLARE_PTR(IntermediateDataBuffer);

}

#endif
