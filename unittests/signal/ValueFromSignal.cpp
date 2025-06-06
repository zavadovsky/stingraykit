// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/signal/signals.h>
#include <stingraykit/signal/ValueFromSignalObtainer.h>
#include <stingraykit/string/ToString.h>

#include <gtest/gtest.h>

#include <list>

using namespace stingray;


void PopulateBool(const function<void (bool)>& slot)
{ slot(true); }


void PopulateIntPtr(const function<void (shared_ptr<int>)>& slot)
{ slot(make_shared_ptr<int>(42)); }


void PopulateVoid(const function<void ()>& slot)
{ slot(); }


void PopulateBoolCollection(const function<void (CollectionOp, bool)>& slot)
{
	slot(CollectionOp::Added, false);
	slot(CollectionOp::Added, true);
}


void PopulateMultiple(const function<void (int, const std::string&, bool)>& slot)
{ slot(42, "test", true); }


TEST(ValueFromSignal, Obtainer)
{
	stingray::signal<void (bool)> s1;
	stingray::signal<void (bool)> s2(&PopulateBool);

	ValueFromSignalObtainer<bool> obtainer;

	s1.SendCurrentState(obtainer);
	ASSERT_EQ(obtainer.HasValue(), false);
	ASSERT_ANY_THROW(obtainer.GetValue());

	s2.SendCurrentState(obtainer);
	ASSERT_EQ(obtainer.HasValue(), true);
	ASSERT_EQ(obtainer.GetValue(), true);

	ASSERT_ANY_THROW(PopulateBool(obtainer));
}


TEST(ValueFromSignal, ObtainerVoid)
{
	stingray::signal<void ()> s1;
	stingray::signal<void ()> s2(&PopulateVoid);

	ValueFromSignalObtainer<void> obtainer;

	s1.SendCurrentState(obtainer);
	ASSERT_EQ(obtainer.HasValue(), false);

	s2.SendCurrentState(obtainer);
	ASSERT_EQ(obtainer.HasValue(), true);

	ASSERT_ANY_THROW(PopulateVoid(obtainer));
}


TEST(ValueFromSignal, Collector)
{
	stingray::signal<void (CollectionOp, bool)> s1;
	stingray::signal<void (CollectionOp, bool)> s2(&PopulateBoolCollection);
	stingray::signal<void (bool)> s3(&PopulateBool);

	ValuesFromSignalCollector<std::list<bool> > collector;

	s1.SendCurrentState(collector);
	ASSERT_EQ(collector.GetValues().size(), 0u);

	s2.SendCurrentState(collector);
	ASSERT_EQ(collector.GetValues().size(), 2u);

	s3.SendCurrentState(collector);
	ASSERT_EQ(collector.GetValues().size(), 3u);
}


TEST(ValueFromSignal, Tuple)
{
	stingray::signal<void (int, const std::string&, bool)> s1;
	stingray::signal<void (int, const std::string&, bool)> s2(&PopulateMultiple);

	TupleFromSignalObtainer<int, std::string, bool> obtainer;

	s1.SendCurrentState(obtainer);
	ASSERT_FALSE(obtainer.HasValues());
	ASSERT_ANY_THROW(obtainer.GetValues());

	s2.SendCurrentState(obtainer);
	ASSERT_TRUE(obtainer.HasValues());
	ASSERT_EQ(obtainer.GetValue<0>(), 42);
	ASSERT_EQ(obtainer.GetValue<1>(), "test");
	ASSERT_EQ(obtainer.GetValue<2>(), true);

	ASSERT_ANY_THROW(PopulateMultiple(obtainer));
}


TEST(ValueFromSignal, Getter)
{
	stingray::signal<void (bool)> s1;
	stingray::signal<void (bool)> s2(&PopulateBool);
	stingray::signal<void (shared_ptr<int>)> s3;
	stingray::signal<void (shared_ptr<int>)> s4(&PopulateIntPtr);

	ASSERT_EQ(HasValueInSignal(s1.connector()), false);
	ASSERT_EQ(GetValueFromSignal(s1.connector()), null);

	ASSERT_EQ(HasValueInSignal(s2.connector()), true);
	ASSERT_EQ(GetValueFromSignal(s2.connector()), true);

	ASSERT_EQ(HasValueInSignal(s3.connector()), false);
	ASSERT_FALSE(GetValueFromSignal(s3.connector()));

	ASSERT_EQ(HasValueInSignal(s4.connector()), true);
	ASSERT_TRUE(GetValueFromSignal(s4.connector()));
	ASSERT_EQ(*GetValueFromSignal(s4.connector()), 42);
}


TEST(ValueFromSignal, GetterVoid)
{
	stingray::signal<void ()> s1;
	stingray::signal<void ()> s2(&PopulateVoid);

	ASSERT_EQ(HasValueInSignal(s1.connector()), false);
	ASSERT_EQ(HasValueInSignal(s2.connector()), true);
}


TEST(ValueFromSignal, CollectorGetter)
{
	stingray::signal<void (CollectionOp, bool)> s1;
	stingray::signal<void (CollectionOp, bool)> s2(&PopulateBoolCollection);

	ASSERT_EQ(GetValuesFromSignal<std::list<bool> >(s1.connector()).size(), 0u);
	ASSERT_EQ(GetValuesFromSignal<std::list<bool> >(s2.connector()).size(), 2u);
}


TEST(ValueFromSignal, TupleGetter)
{
	stingray::signal<void (int, const std::string&, bool)> s1;
	stingray::signal<void (int, const std::string&, bool)> s2(&PopulateMultiple);

	ASSERT_FALSE(HasTupleInSignal(s1.connector()));
	ASSERT_EQ(GetTupleFromSignal(s1.connector()), null);

	ASSERT_TRUE(HasTupleInSignal(s2.connector()));
	ASSERT_TRUE(TupleEquals()(*GetTupleFromSignal(s2.connector()), MakeTuple(42, std::string("test"), true)));
}
