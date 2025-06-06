// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/function/bind.h>
#include <stingraykit/function/functional.h>

#include <unittests/RangeMatcher.h>

#include <gmock/gmock-more-matchers.h>

using namespace stingray;

using ::testing::ElementsAre;
using ::testing::IsEmpty;
using ::testing::Pair;

namespace stingray
{

	template < typename Range_, typename EnableIf<IsRange<Range_>::Value, int>::ValueT = 0 >
	std::ostream& operator << (std::ostream& os, const Range_& val)
	{ return os << ToString(val); }

}

namespace
{

	template<typename R_, typename It_>
	void CheckSequence(R_ r, It_ seqBegin, It_ seqEnd, bool finite)
	{
		for (; r.Valid() && seqBegin != seqEnd; r.Next(), ++seqBegin)
			ASSERT_EQ(r.Get(), *seqBegin);
		ASSERT_TRUE(finite ? !r.Valid() : r.Valid());
		ASSERT_TRUE(seqBegin == seqEnd);
	}


	template<typename RIt_, typename It_>
	void CheckIterators(RIt_ rBegin, RIt_ rEnd, It_ seqBegin, It_ seqEnd)
	{
		for (; rBegin != rEnd && seqBegin != seqEnd; ++rBegin, ++seqBegin)
			ASSERT_EQ(*rBegin, *seqBegin);
		ASSERT_TRUE(rBegin == rEnd);
		ASSERT_TRUE(seqBegin == seqEnd);
	}


	template<typename R_, typename It_, typename Category_>
	void CheckRangeImpl(R_ r, It_ seqBegin, It_ seqEnd, const Category_& category)
	{
		CheckSequence(r, seqBegin, seqEnd, true);
		CheckIterators(r.begin(), r.end(), seqBegin, seqEnd);
	}


	template<typename R_, typename It_>
	void CheckRangeImpl(R_ r, It_ seqBegin, It_ seqEnd, const std::random_access_iterator_tag& category)
	{
		CheckSequence(r, seqBegin, seqEnd, true);
		CheckIterators(r.begin(), r.end(), seqBegin, seqEnd);

		std::ptrdiff_t size = seqEnd - seqBegin;
		ASSERT_EQ(std::ptrdiff_t(r.GetSize()), size);
		ASSERT_EQ(r.end() - r.begin(), size);
	}


	template<typename R_, typename It_>
	void CheckRange(R_ r, It_ seqBegin, It_ seqEnd)
	{ CheckRangeImpl(r, seqBegin, seqEnd, typename R_::Category()); }


	std::string DoZip(int a, const std::string& b, bool c)
	{ return ToString(a) + b + ToString(c); }


	struct Base
	{
		virtual ~Base()
		{ }
	};
	STINGRAYKIT_DECLARE_PTR(Base);


	struct Derived1 : public Base
	{
		int _value;

		Derived1(int value) : _value(value)
		{ }

		int GetValue() const
		{ return _value; }
	};
	STINGRAYKIT_DECLARE_PTR(Derived1);


	struct Derived2 : public Base
	{
		int _value;

		Derived2(int value) : _value(value)
		{ }

		int GetValue() const
		{ return _value; }
	};
	STINGRAYKIT_DECLARE_PTR(Derived2);


	struct InvalidRange : public Range::RangeBase<InvalidRange, bool, std::forward_iterator_tag>
	{
		typedef InvalidRange Self;

		bool Valid() const             			{ return true; }
		typename Self::ValueType Get() const	{ return true; }

		bool Equals(const Self& other) const	{ return false; }
		Self& First()							{ return *this = Self(); }
		Self& Next()                		    { STINGRAYKIT_THROW("Must not iterate"); }
		Self& End()                				{ STINGRAYKIT_THROW("Must not iterate"); }
	};


	template < typename Range_ >
	void StartIterate(Range_ range)
	{
		for (int i : range)
			break;
	}

}

TEST(RangeTest, Ranges)
{
	int initialSequence[] = {1, -42, 5, 42, 42, 37, 0, 53};
	std::vector<int> v(std::begin(initialSequence), std::end(initialSequence));
	std::set<int> s(std::begin(initialSequence), std::end(initialSequence));

	{
		int seq[] = {1, -42, 5, 42, 42, 37, 0, 53};
		CheckRange(ToRange(initialSequence), std::begin(seq), std::end(seq));
	}

	{
		int seq[] = {1, -42, 5, 42, 42, 37, 0, 53};
		CheckRange(ToRange(v), std::begin(seq), std::end(seq));
	}

	{
		int seq[] = {1, -42, 5, 42, 42, 37, 0, 53};
		ASSERT_EQ(std::ptrdiff_t(ToRange(seq) | Count()), std::distance(std::begin(seq), std::end(seq)));
	}

	{
		int seq[] = {1, -42, 5, 37, 0, 53};
		CheckRange(ToRange(v) | Filter(not_(Bind(comparers::Equals(), 42, _1))), std::begin(seq), std::end(seq));
	}

	{
		int seq[] = {53, 0, 37, 42, 42, 5, -42, 1};
		CheckRange(ToRange(v) | Reverse(), std::begin(seq), std::end(seq));
	}

	{
		int seq[] = {2, -41, 6, 43, 43, 38, 1, 54};
		CheckRange(ToRange(v) | Transform(Bind(std::plus<int>(), _1, 1)), std::begin(seq), std::end(seq));
	}

	{
		int seq[] = {54, 1, 38, 43, 43, -41, 2};
		CheckRange(ToRange(v) | Filter(not_(Bind(comparers::Equals(), 5, _1))) | Transform(Bind(std::plus<int>(), _1, 1)) | Reverse(), std::begin(seq), std::end(seq));
	}

	{
		int seq[] = {1, -42, 5, 42, 42, 37, 0, 53};
		CheckRange(ToRange(v) | Drop(0), std::begin(seq), std::end(seq));
	}

	{
		int seq[] = {42, 37, 0, 53};
		CheckRange(ToRange(v) | Drop(4), std::begin(seq), std::end(seq));
	}

	{
		int val;
		CheckRange(ToRange(v) | Drop(8), &val, &val);
	}

	{
		int val;
		CheckRange(ToRange(v) | Drop(16), &val, &val);
	}

	{
		int seq[] = { -42, 0, 1, 5, 37, 42, 53 };
		CheckRange(ToRange(s) | Drop(0), std::begin(seq), std::end(seq));
	}

	{
		int seq[] = { 37, 42, 53 };
		CheckRange(ToRange(s) | Drop(4), std::begin(seq), std::end(seq));
	}

	{
		int val;
		CheckRange(ToRange(s) | Drop(8), &val, &val);
	}

	{
		int val;
		CheckRange(ToRange(v) | Take(0), &val, &val);
	}

	{
		int val;
		CheckRange(ToRange(&val, &val) | Take(4), &val, &val);
	}

	{
		int seq[] = {1, -42, 5, 42};
		CheckRange(ToRange(v) | Take(4), std::begin(seq), std::end(seq));
	}

	{
		int seq[] = {1, -42, 5, 42, 42, 37, 0, 53};
		CheckRange(ToRange(v) | Take(16), std::begin(seq), std::end(seq));
	}

	{
		int val;
		CheckRange(ToRange(s) | Take(0), &val, &val);
	}

	{
		int seq[] = { -42, 0, 1, 5 };
		CheckRange(ToRange(s) | Take(4), std::begin(seq), std::end(seq));
	}

	{
		int seq[] = { -42, 0, 1, 5, 37, 42, 53 };
		CheckRange(ToRange(s) | Take(16), std::begin(seq), std::end(seq));
	}

	{
		int seq[] = {-42, 0, 1, 5, 37, 42, 53};
		CheckRange(ToRange(s), std::begin(seq), std::end(seq));
	}

	ASSERT_EQ(Range::Sum(ToRange(v)), 138);

	ASSERT_EQ(Range::MinElement(ToRange(v)), -42);
	ASSERT_EQ(Range::MaxElement(ToRange(v)), 53);

	ASSERT_TRUE(ToRange(v) | Filter(Bind(comparers::Equals(), 5, _1)) | Any());

	ASSERT_EQ((ToRange(s) += 3).Get(), 5);
}


TEST(RangeTest, First)
{
	{
		const std::vector<int> seq;

		ASSERT_THROW(First(ToRange(seq)), InvalidOperationException);
		ASSERT_THROW(ToRange(seq) | First(), InvalidOperationException);

		ASSERT_EQ(FirstOrDefault(ToRange(seq)), null);
		ASSERT_EQ(ToRange(seq) | FirstOrDefault(), null);

		ASSERT_EQ(FirstOrDefault(ToRange(seq), 12345), 12345);
	}

	{
		const int seq[] = { 1 };

		ASSERT_EQ(First(ToRange(seq)), 1);
		ASSERT_EQ(ToRange(seq) | First(), 1);

		ASSERT_EQ(FirstOrDefault(ToRange(seq)), 1);
		ASSERT_EQ(ToRange(seq) | FirstOrDefault(), 1);

		ASSERT_EQ(FirstOrDefault(ToRange(seq), 12345), 1);
	}

	{
		const int seq[] = { 1, 2, 3, 4, 5 };

		ASSERT_EQ(First(ToRange(seq)), 1);
		ASSERT_EQ(ToRange(seq) | First(), 1);

		ASSERT_EQ(FirstOrDefault(ToRange(seq)), 1);
		ASSERT_EQ(ToRange(seq) | FirstOrDefault(), 1);

		ASSERT_EQ(FirstOrDefault(ToRange(seq), 12345), 1);
	}
}


TEST(RangeTest, Contains)
{
	int r[] = { 1, 3, 5 };

	ASSERT_FALSE(Contains(ToRange(r), 0));
	ASSERT_TRUE(Contains(ToRange(r), 1));
	ASSERT_FALSE(Contains(ToRange(r), 2));
	ASSERT_TRUE(Contains(ToRange(r), 3));
	ASSERT_FALSE(Contains(ToRange(r), 4));
	ASSERT_TRUE(Contains(ToRange(r), 5));
	ASSERT_FALSE(Contains(ToRange(r), 6));

	ASSERT_FALSE(Contains(ToRange(r), Bind(std::less<int>(), _1, 1)));
	ASSERT_TRUE(Contains(ToRange(r), Bind(std::greater<int>(), _1, 2)));
	ASSERT_TRUE(Contains(ToRange(r), Bind(std::less<int>(), _1, 4)));
	ASSERT_FALSE(Contains(ToRange(r), Bind(std::greater<int>(), _1, 5)));
}


TEST(RangeTest, IndexOf)
{
	int r[] = { 1, 3, 5 };

	ASSERT_EQ(IndexOf(ToRange(r), 0), null);
	ASSERT_EQ(IndexOf(ToRange(r), 1), 0u);
	ASSERT_EQ(IndexOf(ToRange(r), 2), null);
	ASSERT_EQ(IndexOf(ToRange(r), 3), 1u);
	ASSERT_EQ(IndexOf(ToRange(r), 4), null);
	ASSERT_EQ(IndexOf(ToRange(r), 5), 2u);
	ASSERT_EQ(IndexOf(ToRange(r), 6), null);

	ASSERT_EQ(IndexOf(ToRange(r), Bind(std::less<int>(), _1, 1)), null);
	ASSERT_EQ(IndexOf(ToRange(r), Bind(std::greater<int>(), _1, 2)), 1u);
	ASSERT_EQ(IndexOf(ToRange(r), Bind(std::less<int>(), _1, 4)), 0u);
	ASSERT_EQ(IndexOf(ToRange(r), Bind(std::greater<int>(), _1, 5)), null);
}


TEST(RangeTest, ElementAt)
{
	{
		const std::vector<int> seq;

		ASSERT_THROW(ElementAt(ToRange(seq), 0), IndexOutOfRangeException);
		ASSERT_THROW(ElementAt(ToRange(seq), 1), IndexOutOfRangeException);
		ASSERT_THROW(ElementAt(ToRange(seq), 4), IndexOutOfRangeException);
		ASSERT_THROW(ElementAt(ToRange(seq), 5), IndexOutOfRangeException);

		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 0), null);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 1), null);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 4), null);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 5), null);

		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 0, 12345), 12345);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 1, 12345), 12345);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 4, 12345), 12345);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 5, 12345), 12345);
	}

	{
		const int seq[] = { 1 };

		ASSERT_EQ(ElementAt(ToRange(seq), 0), 1);
		ASSERT_THROW(ElementAt(ToRange(seq), 1), IndexOutOfRangeException);
		ASSERT_THROW(ElementAt(ToRange(seq), 4), IndexOutOfRangeException);
		ASSERT_THROW(ElementAt(ToRange(seq), 5), IndexOutOfRangeException);

		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 0), 1);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 1), null);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 4), null);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 5), null);

		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 0, 12345), 1);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 1, 12345), 12345);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 4, 12345), 12345);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 5, 12345), 12345);
	}

	{
		const int seq[] = { 1, 2, 3, 4, 5 };

		ASSERT_EQ(ElementAt(ToRange(seq), 0), 1);
		ASSERT_EQ(ElementAt(ToRange(seq), 1), 2);
		ASSERT_EQ(ElementAt(ToRange(seq), 2), 3);
		ASSERT_EQ(ElementAt(ToRange(seq), 3), 4);
		ASSERT_EQ(ElementAt(ToRange(seq), 4), 5);
		ASSERT_THROW(ElementAt(ToRange(seq), 5), IndexOutOfRangeException);

		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 0), 1);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 1), 2);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 2), 3);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 3), 4);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 4), 5);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 5), null);

		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 0, 12345), 1);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 1, 12345), 2);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 2, 12345), 3);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 3, 12345), 4);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 4, 12345), 5);
		ASSERT_EQ(ElementAtOrDefault(ToRange(seq), 5, 12345), 12345);
	}
}


TEST(RangeTest, Fold)
{
	{
		const std::vector<int> seq;

		ASSERT_EQ(Fold(ToRange(seq), std::plus<int>()), null);
		ASSERT_EQ(Fold(ToRange(seq), 10, std::plus<int>()), 10);
	}

	{
		const int seq[] = { 1 };

		ASSERT_EQ(Fold(ToRange(seq), std::plus<int>()), 1);
		ASSERT_EQ(Fold(ToRange(seq), 10, std::plus<int>()), 11);
	}

	{
		const int seq[] = { 1, 2 };

		ASSERT_EQ(Fold(ToRange(seq), std::plus<int>()), 3);
		ASSERT_EQ(Fold(ToRange(seq), 10, std::plus<int>()), 13);
	}

	{
		const int seq[] = { 1, 2, 3, 4, 5 };

		ASSERT_EQ(Fold(ToRange(seq), std::plus<int>()), 15);
		ASSERT_EQ(Fold(ToRange(seq), 10, std::plus<int>()), 25);
	}
}


TEST(RangeTest, Cycle)
{
	int pattern[] = {1, -42, 5};

	int seq[] = {1, -42, 5, 1, -42, 5, 1, -42, 5, 1, -42, 5, 1, -42, 5, 1, -42, 5, 1, -42, 5};
	CheckSequence(Cycle(ToRange(pattern)), std::begin(seq), std::end(seq), false); // TODO: add CycleTransformer
}


TEST(RangeTest, Zip)
{
	{
		int r1[] = { 1, 2, 3 };
		int r2[] = { 4, 5, 6 };

		int seq[] = { 5, 7, 9 };
		CheckSequence(Range::Zip(std::plus<int>(), ToRange(r1), ToRange(r2)), std::begin(seq), std::end(seq), true);
	}

	{
		int r1[] = { 1, 2, 3 };
		int r2[] = { 4, 5, 6, 7 };

		int seq[] = { 5, 7, 9 };
		CheckSequence(Range::Zip(std::plus<int>(), ToRange(r1), ToRange(r2)), std::begin(seq), std::end(seq), true);
	}

	{
		int r1[] = { 1, 2 };
		int r2[] = { 4, 5, 6, 7, 8 };

		int seq[] = { 5, 7, 7, 9, 9 };
		CheckSequence(Range::Zip(std::plus<int>(), Cycle(ToRange(r1)), ToRange(r2)), std::begin(seq), std::end(seq), true);
	}

	{
		int val;
		int r[] = { 4, 5, 6, 7, 8 };

		CheckSequence(Range::Zip(std::plus<int>(), ToRange(&val, &val), ToRange(r)), &val, &val, true);
	}

	{
		int a[] = { 1, 2, 3 };
		std::string b[] = { "a", "b", "c" };
		bool c[] = { true, false, true };

		std::string seq[] = { "1atrue", "2bfalse", "3ctrue" };
		CheckSequence(Range::Zip(&DoZip, ToRange(a), ToRange(b), ToRange(c)), std::begin(seq), std::end(seq), true);
	}
}


TEST(RangeTest, Split)
{
	int r[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	ASSERT_EQ(Range::Split(std::begin(r), std::end(r), 1) | Count(), 9u);
	ASSERT_EQ(Range::Split(std::begin(r), std::end(r), 2) | Count(), 5u);
	ASSERT_EQ(Range::Split(std::begin(r), std::end(r), 3) | Count(), 3u);
	ASSERT_EQ(Range::Split(std::begin(r), std::end(r), 10) | Count(), 1u);
}


TEST(RangeTest, Flatten)
{
	const std::vector<std::vector<int>> v = { { 0 }, { 1, 2 }, { }, { 3, 4 } };
	const std::vector<std::vector<int>> vEmpty;
	const std::vector<std::vector<int>> vEmpty2 = { { } };
	const std::vector<std::string> strings = { "ab", "", "cd" };

	ASSERT_THAT(Range::Flatten(ToRange(v)), MatchRange(ElementsAre(0, 1, 2, 3, 4)));
	ASSERT_THAT(Range::Flatten(ToRange(v)) | Reverse(), MatchRange(ElementsAre(4, 3, 2, 1, 0)));
	ASSERT_THAT(Range::Flatten(ToRange(v) | Reverse()), MatchRange(ElementsAre(3, 4, 1, 2, 0)));

	ASSERT_THAT(Range::Flatten(ToRange(v).Next()), MatchRange(ElementsAre(1, 2, 3, 4)));

	ASSERT_THAT(Range::Flatten(ToRange(vEmpty)), MatchRange(IsEmpty()));
	ASSERT_THAT(Range::Flatten(ToRange(vEmpty)) | Reverse(), MatchRange(IsEmpty()));

	ASSERT_THAT(Range::Flatten(ToRange(vEmpty2)), MatchRange(IsEmpty()));
	ASSERT_THAT(Range::Flatten(ToRange(vEmpty2)) | Reverse(), MatchRange(IsEmpty()));

	ASSERT_THAT(Range::Flatten(Range::Split(std::vector<int>{ 0, 1, 2 }, 1)), MatchRange(ElementsAre(0, 1, 2)));

	{
		const auto range = Range::Flatten(ToRange(strings));
		ASSERT_EQ(std::string(range.begin(), range.end()), "abcd");
	}

	{
		const auto range = Range::Flatten(ToRange(strings)) | Reverse();
		ASSERT_EQ(std::string(range.begin(), range.end()), "dcba");
	}

	{
		const auto range = Range::Flatten(ToRange(strings) | Reverse());
		ASSERT_EQ(std::string(range.begin(), range.end()), "cdab");
	}
}


TEST(RangeTest, Polymorphic)
{
	std::vector<BasePtr> v;
	v.push_back(make_shared_ptr<Derived1>(1));
	v.push_back(make_shared_ptr<Derived1>(2));
	v.push_back(make_shared_ptr<Derived1>(3));
	v.push_back(make_shared_ptr<Derived1>(42));

	{
		int seq[] = {1, 2, 3, 42};
		CheckRange(ToRange(v) | Cast<Derived1Ptr>() | Transform(&Derived1::GetValue), std::begin(seq), std::end(seq));
	}

	ASSERT_TRUE((ToRange(v) | Cast<Derived2Ptr>()).Valid());
	ASSERT_ANY_THROW((ToRange(v) | Cast<Derived2Ptr>()).Get());

	v.push_back(make_shared_ptr<Derived2>(53));
	v.push_back(make_shared_ptr<Derived2>(1));
	v.push_back(make_shared_ptr<Derived1>(37));
	v.push_back(make_shared_ptr<Derived2>(7));

	{
		int seq[] = {1, 2, 3, 42, 37};
		CheckRange(ToRange(v) | OfType<Derived1Ptr>() | Transform(&Derived1::GetValue), std::begin(seq), std::end(seq));
	}

	{
		int seq[] = {53, 1, 7};
		CheckRange(ToRange(v) | OfType<Derived2Ptr>() | Transform(&Derived2::GetValue), std::begin(seq), std::end(seq));
	}
}


TEST(RangeTest, ForBasedLoop)
{
	ASSERT_ANY_THROW(StartIterate(InvalidRange()));
	ASSERT_NO_THROW(StartIterate(IterableRange(InvalidRange())));

	{
		int r[] = { 0, 1, 2, 3, 4 };
		int j = 0;
		for (int i : IterableRange(ToRange(r)))
			ASSERT_EQ(j++, i);

		ASSERT_EQ(j, 5);
	}
}


TEST(RangeTest, Filter)
{
	std::vector<bool> empty;
	std::vector<bool> invalid = { false };
	std::vector<int> mixed = { 0, 1, 2, 0 };

	ASSERT_THAT(ToRange(empty) | Filter(&implicit_cast<bool>), MatchRange(ElementsAre()));
	ASSERT_THAT(ToRange(empty) | Filter(&implicit_cast<bool>) | Reverse(), MatchRange(ElementsAre()));

	auto emptyRange = ToRange(empty) | Filter(&implicit_cast<bool>);
	ASSERT_FALSE(emptyRange.Valid());
	ASSERT_FALSE(emptyRange.Last().Valid());
	ASSERT_EQ(emptyRange.begin(), emptyRange.end());

	ASSERT_THAT(ToRange(invalid) | Filter(&implicit_cast<bool>), MatchRange(ElementsAre()));
	ASSERT_THAT(ToRange(invalid) | Filter(&implicit_cast<bool>) | Reverse(), MatchRange(ElementsAre()));

	auto invalidRange = ToRange(invalid) | Filter(&implicit_cast<bool>);
	ASSERT_FALSE(invalidRange.Valid());
	ASSERT_FALSE(invalidRange.Last().Valid());
	ASSERT_EQ(invalidRange.begin(), invalidRange.end());

	ASSERT_THAT(ToRange(mixed) | Filter(&implicit_cast<bool>), MatchRange(ElementsAre(1, 2)));
	ASSERT_THAT(ToRange(mixed) | Filter(&implicit_cast<bool>) | Reverse(), MatchRange(ElementsAre(2, 1)));

	auto mixedRange = ToRange(mixed) | Filter(&implicit_cast<bool>);
	ASSERT_TRUE(mixedRange.Valid());
	ASSERT_TRUE(mixedRange.Last().Valid());
	ASSERT_EQ(std::next(mixedRange.begin(), 2), mixedRange.end());
}


TEST(RangeTest, OfType)
{
	std::vector<BasePtr> empty;
	std::vector<BasePtr> invalid = { make_shared_ptr<Derived2>(0) };
	std::vector<BasePtr> mixed = { make_shared_ptr<Derived2>(0), make_shared_ptr<Derived1>(1), make_shared_ptr<Derived1>(2), make_shared_ptr<Derived2>(0) };

	ASSERT_THAT(ToRange(empty) | OfType<Derived1Ptr>() | Transform(&Derived1::GetValue), MatchRange(ElementsAre()));
	ASSERT_THAT(ToRange(empty) | OfType<Derived1Ptr>() | Reverse() | Transform(&Derived1::GetValue), MatchRange(ElementsAre()));

	auto emptyRange = ToRange(empty) | OfType<Derived1Ptr>() | Transform(&Derived1::GetValue);
	ASSERT_FALSE(emptyRange.Valid());
	ASSERT_FALSE(emptyRange.Last().Valid());
	ASSERT_EQ(emptyRange.begin(), emptyRange.end());

	ASSERT_THAT(ToRange(invalid) | OfType<Derived1Ptr>() | Transform(&Derived1::GetValue), MatchRange(ElementsAre()));
	ASSERT_THAT(ToRange(invalid) | OfType<Derived1Ptr>() | Reverse() | Transform(&Derived1::GetValue), MatchRange(ElementsAre()));

	auto invalidRange = ToRange(invalid) | OfType<Derived1Ptr>() | Transform(&Derived1::GetValue);
	ASSERT_FALSE(invalidRange.Valid());
	ASSERT_FALSE(invalidRange.Last().Valid());
	ASSERT_EQ(invalidRange.begin(), invalidRange.end());

	ASSERT_THAT(ToRange(mixed) | OfType<Derived1Ptr>() | Transform(&Derived1::GetValue), MatchRange(ElementsAre(1, 2)));
	ASSERT_THAT(ToRange(mixed) | OfType<Derived1Ptr>() | Reverse() | Transform(&Derived1::GetValue), MatchRange(ElementsAre(2, 1)));

	auto mixedRange = ToRange(mixed) | OfType<Derived1Ptr>() | Transform(&Derived1::GetValue);
	ASSERT_TRUE(mixedRange.Valid());
	ASSERT_TRUE(mixedRange.Last().Valid());
	ASSERT_EQ(std::next(mixedRange.begin(), 2), mixedRange.end());
}


TEST(RangeTest, Concat)
{
	const std::vector<int> v1{ 0, 1, 2 };
	const std::vector<int> v2{ 3, 4 };
	const std::vector<int> vEmpty;

	ASSERT_THAT(Concat(ToRange(v1), ToRange(v2)), MatchRange(ElementsAre(0, 1, 2, 3, 4)));
	ASSERT_THAT(Concat(ToRange(v1), ToRange(v2)) | Reverse(), MatchRange(ElementsAre(4, 3, 2, 1, 0)));

	ASSERT_THAT(Concat(ToRange(vEmpty), ToRange(vEmpty)), MatchRange(IsEmpty()));
	ASSERT_THAT(Concat(ToRange(vEmpty), ToRange(vEmpty)) | Reverse(), MatchRange(IsEmpty()));

	ASSERT_THAT(Concat(ToRange(vEmpty), ToRange(v1), ToRange(v2)), MatchRange(ElementsAre(0, 1, 2, 3, 4)));
	ASSERT_THAT(Concat(ToRange(vEmpty), ToRange(v1), ToRange(v2)) | Reverse(), MatchRange(ElementsAre(4, 3, 2, 1, 0)));

	ASSERT_THAT(Concat(ToRange(v1), ToRange(vEmpty), ToRange(v2)), MatchRange(ElementsAre(0, 1, 2, 3, 4)));
	ASSERT_THAT(Concat(ToRange(v1), ToRange(vEmpty), ToRange(v2)) | Reverse(), MatchRange(ElementsAre(4, 3, 2, 1, 0)));

	ASSERT_THAT(Concat(ToRange(v1), ToRange(v2), ToRange(vEmpty)), MatchRange(ElementsAre(0, 1, 2, 3, 4)));
	ASSERT_THAT(Concat(ToRange(v1), ToRange(v2), ToRange(vEmpty)) | Reverse(), MatchRange(ElementsAre(4, 3, 2, 1, 0)));

	ASSERT_THAT(Concat(ToRange(v1) | Filter(Bind(std::less<int>(), 0, _1)), ToRange(v2)), MatchRange(ElementsAre(1, 2, 3, 4)));
	ASSERT_THAT(Concat(ToRange(v1) | Filter(Bind(std::less<int>(), 0, _1)), ToRange(v2)) | Reverse(), MatchRange(ElementsAre(4, 3, 2, 1)));

	ASSERT_THAT(Concat(ToRange(v1), ToRange(v2) | Filter(Bind(std::less<int>(), 3, _1))), MatchRange(ElementsAre(0, 1, 2, 4)));
	ASSERT_THAT(Concat(ToRange(v1), ToRange(v2) | Filter(Bind(std::less<int>(), 3, _1))) | Reverse(), MatchRange(ElementsAre(4, 2, 1, 0)));
}


TEST(RangeTest, Single)
{
	int val = 1;
	ASSERT_THAT(Range::Single(val), MatchRange(ElementsAre(1)));
	ASSERT_THAT(Range::Single(val) | Filter(Bind(std::greater<int>(), 3, _1)), MatchRange(ElementsAre(1)));
	ASSERT_THAT(Range::Single(val) | Filter(Bind(std::less<int>(), 3, _1)), MatchRange(IsEmpty()));
	ASSERT_THAT(Range::Single(val) | Reverse(), MatchRange(ElementsAre(1)));
	ASSERT_THAT(Range::Single(val) | Drop(0), MatchRange(ElementsAre(1)));
	ASSERT_THAT(Range::Single(val) | Drop(1), MatchRange(IsEmpty()));

	std::string str = "test";
	ASSERT_THAT(Range::Single(str), MatchRange(ElementsAre("test")));
	ASSERT_THAT(Range::Single("test"), MatchRange(ElementsAre("test")));
	ASSERT_THAT(Range::Single(std::string("test")), MatchRange(ElementsAre("test")));

	const auto range = Range::Single<std::pair<int, std::string>>(1, "test");
	ASSERT_THAT(range, MatchRange(ElementsAre(Pair(1, "test"))));
}
