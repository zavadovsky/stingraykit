#ifndef STINGRAY_STINGRAYKIT_STINGRAYKIT_IO_BUFFEREDDATASOURCE_H
#define STINGRAY_STINGRAYKIT_STINGRAYKIT_IO_BUFFEREDDATASOURCE_H

#include <stingraykit/io/IDataSource.h>
#include <stingraykit/io/SharedCircularBuffer.h>
#include <stingraykit/log/Logger.h>

namespace stingray
{

	class BufferedDataSource : public virtual IDataSource
	{
	private:
		static NamedLogger		s_logger;

		SharedCircularBufferPtr	_buffer;
		const size_t			_outputPacketSize;

	public:
		BufferedDataSource(const SharedCircularBufferPtr& buffer, size_t outputPacketSize)
			:	_buffer(STINGRAYKIT_REQUIRE_NOT_NULL(buffer)),
				_outputPacketSize(outputPacketSize)
		{
			STINGRAYKIT_CHECK(outputPacketSize != 0, ArgumentException("outputPacketSize", outputPacketSize));
			STINGRAYKIT_CHECK(_buffer->_buffer.GetTotalSize() % outputPacketSize == 0, "Buffer size is not a multiple of output packet size!");
		}

		void Read(IDataConsumer& consumer, const ICancellationToken& token) override
		{
			SharedCircularBuffer::BufferLock bl(*_buffer);
			SharedCircularBuffer::ReadLock rl(*_buffer);

			BithreadCircularBuffer::Reader r = _buffer->_buffer.Read();

			size_t packetized_size = r.size() / _outputPacketSize * _outputPacketSize;
			if (packetized_size == 0)
			{
				if (_buffer->_exception)
					RethrowException(_buffer->_exception);
				else if (_buffer->_eod)
				{
					if (r.size() != 0)
						s_logger.Warning() << "Dropping " << r.size() << " bytes from DataBuffer - end of data!";

					consumer.EndOfData(token);
					return;
				}

				_buffer->_bufferEmpty.Wait(_buffer->_bufferMutex, token);
				return;
			}

			size_t processed_size = 0;
			{
				SharedCircularBuffer::BufferUnlock ul(bl);
				processed_size = consumer.Process(ConstByteData(r.GetData(), 0, packetized_size), token);
			}

			if (processed_size == 0)
				return;

			if (processed_size % _outputPacketSize != 0)
			{
				s_logger.Error() << "Processed size: " << processed_size << " is not a multiple of output packet size: " << _outputPacketSize;
				processed_size = packetized_size;
			}

			r.Pop(processed_size);
			_buffer->_bufferFull.Broadcast();
		}

		size_t GetOutputPacketSize() const
		{ return _outputPacketSize; }
	};

}

#endif
