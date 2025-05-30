#ifndef STINGRAYKIT_OBSERVABLEVALUE_H
#define STINGRAYKIT_OBSERVABLEVALUE_H

// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/compare/comparers.h>
#include <stingraykit/signal/signals.h>
#include <stingraykit/IObservableValue.h>

namespace stingray
{

	namespace ObservableValuePolicies
	{

		template < typename T >
		struct ConditionalPopulationPolicySpecifier
		{
			T	DefaultValue;

			explicit ConditionalPopulationPolicySpecifier(typename GetParamPassingType<T>::ValueT defaultValue = T()) : DefaultValue(defaultValue) { }

			template < typename U >
			operator ConditionalPopulationPolicySpecifier<U>() const
			{ return ConditionalPopulationPolicySpecifier<U>(DefaultValue); }
		};

		template < typename T >
		ConditionalPopulationPolicySpecifier<T> PopulateIfDiffers(const T& defaultValue = T())
		{ return ConditionalPopulationPolicySpecifier<T>(defaultValue); }

		template < typename T, typename EqualsCmp >
		class ConditionalPopulation
		{
		public:
			using Specifier = ConditionalPopulationPolicySpecifier<T>;

		private:
			T		_defaultValue;

		public:
			template < typename U >
			explicit ConditionalPopulation(const ConditionalPopulationPolicySpecifier<U>& specifier) : _defaultValue(specifier.DefaultValue) { }

			bool NeedPopulate(typename GetParamPassingType<T>::ValueT value) const
			{ return !EqualsCmp()(value, _defaultValue); }
		};

		template < typename T, typename EqualsCmp >
		struct MandatoryPopulation
		{
			using Specifier = EmptyType;

			explicit MandatoryPopulation(const Specifier&) { }

			bool NeedPopulate(typename GetParamPassingType<T>::ValueT) const
			{ return true; }
		};

		template < typename T, typename EqualsCmp >
		struct DisabledPopulation
		{
			using Specifier = EmptyType;

			explicit DisabledPopulation(const Specifier&) { }

			bool NeedPopulate(typename GetParamPassingType<T>::ValueT) const
			{ return false; }
		};

	}


	template < typename T, typename EqualsCmp = comparers::Equals, template < typename, typename > class PopulationPolicy = ObservableValuePolicies::MandatoryPopulation>
	class ObservableValue final : public virtual IObservableValue<T>
	{
		static_assert(comparers::IsEqualsComparer<EqualsCmp>::Value, "Expected Equals comparer");

		STINGRAYKIT_NONCOPYABLE(ObservableValue);

	private:
		using Base = IObservableValue<T>;

		using ExternalMutexPointer = signal_policies::threading::ExternalMutexPointer;

		using PopulationPolicySpecifier = typename PopulationPolicy<T, EqualsCmp>::Specifier;

	public:
		using ParamPassingType = typename Base::ParamPassingType;
		using OnChangedSignature = typename Base::OnChangedSignature;

	private:
		T													_val;
		EqualsCmp											_equalsCmp;
		const shared_ptr<const Mutex>						_mutex;
		PopulationPolicy<T, EqualsCmp>						_populationPolicy;
		signal<OnChangedSignature, ExternalMutexPointer>	_onChanged;

	public:
		explicit ObservableValue(ParamPassingType val = T(), const PopulationPolicySpecifier& populationPolicySpecifier = PopulationPolicySpecifier())
			:	ObservableValue(make_shared_ptr<Mutex>(), val, populationPolicySpecifier)
		{ }

		explicit ObservableValue(const shared_ptr<const Mutex>& mutex, ParamPassingType val = T(), const PopulationPolicySpecifier& populationPolicySpecifier = PopulationPolicySpecifier())
			:	_val(val),
				_mutex(STINGRAYKIT_REQUIRE_NOT_NULL(mutex)),
				_populationPolicy(populationPolicySpecifier),
				_onChanged(ExternalMutexPointer(_mutex), Bind(&ObservableValue::OnChangedPopulator, this, _1))
		{ }

		ObservableValue& operator = (ParamPassingType val)
		{
			Set(val);
			return *this;
		}

		operator T () const
		{ return Get(); }

		void Set(ParamPassingType val) override
		{
			signal_locker l(_onChanged);
			if (_equalsCmp(_val, val))
				return;
			_val = val;
			_onChanged(_val);
		}

		T Get() const override
		{
			signal_locker l(_onChanged);
			return _val;
		}

		signal_connector<OnChangedSignature> OnChanged() const override
		{ return _onChanged.connector(); }

		const Mutex& GetSyncRoot() const override
		{ return *_mutex; }

	private:
		void OnChangedPopulator(const function<OnChangedSignature>& slot) const
		{ if (_populationPolicy.NeedPopulate(_val)) slot(_val); }
	};

}

#endif
