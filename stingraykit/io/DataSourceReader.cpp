// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/io/DataSourceReader.h>

#include <stingraykit/io/DataSources.h>

namespace stingray
{

	namespace
	{

		class DataStorage final : public virtual IDataConsumer
		{
		private:
			ByteArray	_data;

		public:
			size_t Process(ConstByteData data, const ICancellationToken&) override
			{
				_data.append(data.begin(), data.end());
				return data.size();
			}

			void EndOfData(const ICancellationToken&) override { }

			ByteArray GetData() const { return _data; }
		};

	}


	DataSourceReader::DataSourceReader(const IDataSourcePtr& source)
		: _source(STINGRAYKIT_REQUIRE_NOT_NULL(source))
	{ }


	ByteArray DataSourceReader::ReadToEnd(const ICancellationToken& token)
	{
		DataStorage storage;
		ReactiveDataSource(_source).Read(storage, token);
		return storage.GetData();
	}

}
