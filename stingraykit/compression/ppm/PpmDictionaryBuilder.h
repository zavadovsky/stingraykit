#ifndef STINGRAYKIT_COMPRESSION_PPM_PPMDICTIONARYBUILDER_H
#define STINGRAYKIT_COMPRESSION_PPM_PPMDICTIONARYBUILDER_H

// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/compression/ICompressionDictionary.h>
#include <stingraykit/compression/ppm/PpmDictionary.h>
#include <stingraykit/compression/ppm/PpmModel.h>

#include <deque>

namespace stingray
{

	template <typename PpmConfig_, typename ModelConfigList_>
	class PpmDictionaryBuilder : public virtual ICompressionDictionaryBuilder
	{
	public:
		typedef PpmImpl<PpmConfig_>                                       Impl;
		typedef typename Impl::template ModelBuilder<ModelConfigList_>    ModelBuilder;
		typedef typename Impl::template Model<ModelConfigList_>           Model;
		typedef typename Impl::template Compactificator<ModelConfigList_> Compactificator;
		typedef PpmDictionary<PpmConfig_, ModelConfigList_>               Dictionary;

	private:
		const size_t                      _lowMemoryThreshold;
		const size_t                      _highMemoryThreshold;
		const size_t                      _finalMemoryThreshold;
		optional<ModelBuilder>            _model;
		std::deque<typename Impl::Symbol> _context;

	public:
		PpmDictionaryBuilder(size_t lowMemoryThreshold, size_t highMemoryThreshold, size_t finalMemoryThreshold) :
			_lowMemoryThreshold(lowMemoryThreshold), _highMemoryThreshold(highMemoryThreshold),
			_finalMemoryThreshold(finalMemoryThreshold)
		{ _model.emplace(); }

		virtual void Process(ConstByteData data)
		{
			for (ConstByteData::const_iterator it = data.begin(); it != data.end(); ++it)
				AddSymbol(*it);
		}

		virtual ICompressionDictionaryPtr Finish()
		{
			Compactificator compactificator(*_model);
			while (_model->GetMemoryConsumption(typename Impl::ModelMemoryCounter()) > _finalMemoryThreshold)
				compactificator.Compactify(*compactificator.GetMinimalCount());

			shared_ptr<Model> model(make_shared_ptr<Model>(*_model));
			_model.emplace();
			_context.clear();
			return make_shared_ptr<Dictionary>(model);
		}

	private:
		void AddSymbol(typename Impl::Symbol symbol)
		{
			_model->AddSymbol(_context, symbol, typename Impl::SymbolCount(1));
			UpdateContext(symbol);

			if (_model->GetMemoryConsumption(typename Impl::ModelBuilderMemoryCounter()) > _highMemoryThreshold)
				Compactify();
		}

		void UpdateContext(typename Impl::Symbol symbol)
		{
			_context.push_back(symbol);
			if (_context.size() > Model::ContextSize)
				_context.pop_front();
		}

		void Compactify()
		{
			Compactificator compactificator(*_model);
			while (_model->GetMemoryConsumption(typename Impl::ModelBuilderMemoryCounter()) > _lowMemoryThreshold)
				compactificator.Compactify(*compactificator.GetMinimalCount());
		}
	};


}

#endif
