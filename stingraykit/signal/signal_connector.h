#ifndef STINGRAYKIT_SIGNAL_SIGNAL_CONNECTOR_H
#define STINGRAYKIT_SIGNAL_SIGNAL_CONNECTOR_H

// Copyright (c) 2011 - 2024, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/function/AsyncFunction.h>
#include <stingraykit/signal/signal_policies.h>
#include <stingraykit/Token.h>

namespace stingray
{

	/**
	 * @addtogroup toolkit_functions
	 * @{
	 */

	namespace Detail
	{

		struct ISignalConnector : public self_counter<ISignalConnector>
		{
			virtual ~ISignalConnector() { }

			virtual Token Connect(const function_storage& func, const FutureExecutionTester& invokeTester, TaskLifeToken&& connectionToken, bool sendCurrentState) = 0;
			virtual void SendCurrentState(const function_storage& slot) const = 0;

			virtual TaskLifeToken CreateSyncToken() const = 0;
			virtual TaskLifeToken CreateAsyncToken() const = 0;

			virtual ConnectionPolicy GetConnectionPolicy() const = 0;
		};

	}


	template < typename Signature_ >
	class signal_connector
	{
	public:
		using Signature = Signature_;

		using RetType = void;
		using ParamTypes = typename function_info<Signature>::ParamTypes;

	private:
		self_count_ptr<Detail::ISignalConnector>	_impl;

	public:
		signal_connector() { }
		explicit signal_connector(const self_count_ptr<Detail::ISignalConnector>& impl) : _impl(STINGRAYKIT_REQUIRE_NOT_NULL(impl)) { }

		void SendCurrentState(const function<Signature_>& slot) const
		{
			if (STINGRAYKIT_LIKELY(_impl.is_initialized()))
				_impl->SendCurrentState(function_storage(slot));
		}

		Token connect(const function<Signature_>& slot, bool sendCurrentState = true) const
		{
			if (STINGRAYKIT_UNLIKELY(!_impl))
				return Token();

			STINGRAYKIT_CHECK(_impl->GetConnectionPolicy() == ConnectionPolicy::Any || _impl->GetConnectionPolicy() == ConnectionPolicy::SyncOnly, "sync-connect to async-only signal");

			TaskLifeToken token(_impl->CreateSyncToken());
			const FutureExecutionTester tester(token.GetExecutionTester());

			return _impl->Connect(function_storage(slot), tester, std::move(token), sendCurrentState);
		}

		Token connect(const ITaskExecutorPtr& worker, const function<Signature_>& slot, bool sendCurrentState = true) const
		{
			if (STINGRAYKIT_UNLIKELY(!_impl))
				return Token();

			STINGRAYKIT_CHECK(_impl->GetConnectionPolicy() == ConnectionPolicy::Any || _impl->GetConnectionPolicy() == ConnectionPolicy::AsyncOnly, "async-connect to sync-only signal");

			TaskLifeToken token(_impl->CreateAsyncToken());
			const FutureExecutionTester tester(token.GetExecutionTester());

			return _impl->Connect(function_storage(function<Signature_>(MakeAsyncFunction(worker, slot, tester))), null, std::move(token), sendCurrentState);
		}
	};


	namespace Detail
	{

		struct DummySignalConnectorProxy
		{
			template < typename Signature_ >
			operator signal_connector<Signature_> () const
			{ return signal_connector<Signature_>(); }
		};

	}


	inline Detail::DummySignalConnectorProxy make_dummy_signal_connector()
	{ return Detail::DummySignalConnectorProxy(); }

	/** @} */

}

#endif
