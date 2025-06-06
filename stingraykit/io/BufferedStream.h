#ifndef STINGRAYKIT_IO_BUFFEREDSTREAM_H
#define STINGRAYKIT_IO_BUFFEREDSTREAM_H

// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/io/IByteStream.h>
#include <stingraykit/string/ToString.h>
#include <stingraykit/math.h>

#include <string.h>

namespace stingray
{

	class BufferedStream : public virtual IByteStream
	{
	public:
		static const size_t DefaultBufferSize = 4096;
		static const size_t DefaultAlignment  = 1;

	private:
		IByteStreamPtr _stream;
		ByteArray      _buffer;
		size_t         _bufferSize;

		u64            _bufferOffset;
		size_t         _inBufferOffset;

		size_t         _alignment;

	public:
		BufferedStream(const IByteStreamPtr& stream, size_t bufferSize = DefaultBufferSize, size_t alignment = DefaultAlignment):
			_stream(stream), _buffer(bufferSize), _bufferSize(0), _bufferOffset(0), _inBufferOffset(0), _alignment(alignment)
		{ STINGRAYKIT_CHECK(bufferSize % alignment == 0, ArgumentException("alignment", alignment)); }

		virtual u64 Read(ByteData data, const ICancellationToken& token)
		{
			const size_t dstSize = data.size();
			size_t total = 0;
			u8* dst = data.data();
			for (size_t toCopy = 0; total < dstSize && token; _inBufferOffset += toCopy, dst += toCopy, total += toCopy)
			{
				if (!_bufferSize || _inBufferOffset == _bufferSize)
				{
					if (_bufferSize)
						SeekStream(Tell());
					_bufferSize = _stream->Read(_buffer, token);
					if (!_bufferSize || _inBufferOffset == _bufferSize)
						break;
				}
				toCopy = std::min(dstSize - total, _bufferSize - _inBufferOffset);
				::memcpy(dst, _buffer.data() + _inBufferOffset, toCopy);
			}
			return total;
		}

		virtual u64 Write(ConstByteData data, const ICancellationToken& token)
		{
			if (_inBufferOffset != _bufferSize || _bufferSize)
				_stream->Seek(Tell());
			u64 total = _stream->Write(data, token);
			SeekStream(Tell() + total);
			return total;
		}

		virtual void Seek(s64 offset, SeekMode mode = SeekMode::Begin)
		{
			switch (mode)
			{
			case SeekMode::Current:
				offset += Tell();
				// fallthrough
			case SeekMode::Begin:
				if (!SeekInBuffer(static_cast<u64>(offset)))
					SeekStream(static_cast<u64>(offset));
				return;
			case SeekMode::End:
				_stream->Seek(offset, mode);
				offset = _stream->Tell();
				if (SeekInBuffer(static_cast<u64>(offset)))
					_stream->Seek(_bufferOffset);
				else
					SeekStream(static_cast<u64>(offset));
			}
		}

		virtual u64 Tell() const
		{ return _bufferOffset + _inBufferOffset; }

	private:
		bool SeekInBuffer(u64 offset)
		{
			if (_bufferOffset <= offset && offset - _bufferOffset < static_cast<u64>(_bufferSize))
			{
				_inBufferOffset = static_cast<size_t>(offset - _bufferOffset);
				return true;
			}
			return false;
		}

		void SeekStream(u64 offset)
		{
			const u64 alignedOffset(AlignDown<u64>(offset, _alignment));
			_stream->Seek(alignedOffset);
			_bufferOffset = alignedOffset;
			_inBufferOffset = static_cast<size_t>(offset - alignedOffset);
			_bufferSize = 0;
		}
	};
	STINGRAYKIT_DECLARE_PTR(BufferedStream);

}
#endif
