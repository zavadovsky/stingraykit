// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/collection/flat_map.h>

#include <gmock/gmock-matchers.h>

using namespace stingray;

using ::testing::ElementsAre;

namespace
{

	using Vector = std::vector<std::pair<std::string, std::string>>;
	using Map = std::map<std::string, std::string>;
	using FlatMap = flat_map<std::string, std::string>;
	using TransparentFlatMap = flat_map<std::string, std::string, std::less<>>;

	Vector GetUnorderedVector()
	{
		return { { "one", "jaws" }, { "two", "bite" }, { "three", "claws" }, { "four", "catch" }, { "five", "blow" },
				{ "six", "job" }, { "seven", "two" }, { "eight", "girls" }, { "nine", "one" }, { "ten", "cup"} };
	}

	FlatMap GetSampleFlatMap()
	{
		return { { "Americans", "God blessed nation!" }, { "Australians", "Got no idea why they don't fall!" }, { "Japaneses", "Nation presented us anime!" },
				{ "Russians", "Greatest nation in the world!" }, { "Ukrainians", "Salo Ukraine! Geroyam Salo!" } };
	}

	struct PairEquals
	{
		template < typename Lhs_, typename Rhs_ >
		bool operator () (const Lhs_& lhs, const Rhs_& rhs) const
		{ return lhs.first == rhs.first && lhs.second == rhs.second; }
	};

}


TEST(FlatMapTest, Construction)
{
	{
		FlatMap testee;
		EXPECT_TRUE(testee.empty());
	}
	{
		FlatMap testee((FlatMap::key_compare()));
		EXPECT_TRUE(testee.empty());
	}
	{
		FlatMap testee((FlatMap::key_compare()), (FlatMap::allocator_type()));
		EXPECT_TRUE(testee.empty());
	}
	{
		FlatMap testee((FlatMap::allocator_type()));
		EXPECT_TRUE(testee.empty());
	}
	{
		const Vector vec = { { "one", "jaws" }, { "two", "bite" }, { "three", "claws" }, { "four", "catch" }, { "four", "dup" }, { "three", "dup" } };

		FlatMap testee(vec.begin(), vec.end());
		EXPECT_EQ(testee.size(), (size_t)4);
		EXPECT_TRUE(std::is_sorted(testee.begin(), testee.end(), testee.value_comp()));

		ASSERT_THAT(testee, ElementsAre(std::make_pair("four", "catch"), std::make_pair("one", "jaws"), std::make_pair("three", "claws"), std::make_pair("two", "bite")));
	}
	{
		const Map map = { { "one", "jaws" }, { "two", "bite" }, { "three", "claws" }, { "four", "catch" }, { "four", "dup" }, { "three", "dup" } };

		FlatMap testee(map.begin(), map.end());
		EXPECT_TRUE(std::is_sorted(testee.begin(), testee.end(), testee.value_comp()));
		EXPECT_TRUE(std::equal(map.begin(), map.end(), testee.begin(), testee.end(), PairEquals()));

		ASSERT_THAT(testee, ElementsAre(std::make_pair("four", "catch"), std::make_pair("one", "jaws"), std::make_pair("three", "claws"), std::make_pair("two", "bite")));
	}
	{
		FlatMap testee1 = GetSampleFlatMap();

		FlatMap testee2(testee1, FlatMap::allocator_type());
		EXPECT_FALSE(testee1.empty());

		FlatMap testee3(std::move(testee1), FlatMap::allocator_type());
		EXPECT_TRUE(testee1.empty());

		EXPECT_TRUE(std::equal(testee2.begin(), testee2.end(), testee3.begin(), testee3.end()));

		ASSERT_THAT(testee2, ElementsAre(std::make_pair("Americans", "God blessed nation!"), std::make_pair("Australians", "Got no idea why they don't fall!"),
				std::make_pair("Japaneses", "Nation presented us anime!"), std::make_pair("Russians", "Greatest nation in the world!"), std::make_pair("Ukrainians", "Salo Ukraine! Geroyam Salo!")));
	}
}


TEST(FlatMapTest, Assignment)
{
	{
		const FlatMap testee1 = GetSampleFlatMap();

		FlatMap testee2;
		FlatMap testee3;
		testee2 = testee1;
		testee3 = testee1;

		EXPECT_TRUE(std::equal(testee2.begin(), testee2.end(), testee3.begin(), testee3.end(), PairEquals()));

		ASSERT_THAT(testee2, ElementsAre(std::make_pair("Americans", "God blessed nation!"), std::make_pair("Australians", "Got no idea why they don't fall!"),
				std::make_pair("Japaneses", "Nation presented us anime!"), std::make_pair("Russians", "Greatest nation in the world!"), std::make_pair("Ukrainians", "Salo Ukraine! Geroyam Salo!")));
	}
	{
		const std::initializer_list<std::pair<std::string, std::string>> values{ { "one", "jaws" }, { "two", "bite" }, { "three", "claws" }, { "four", "catch" }, { "four", "dup" }, { "three", "dup" } };

		FlatMap testee;
		testee = values;

		EXPECT_TRUE(std::is_sorted(testee.begin(), testee.end(), testee.value_comp()));

		ASSERT_THAT(testee, ElementsAre(std::make_pair("four", "catch"), std::make_pair("one", "jaws"), std::make_pair("three", "claws"), std::make_pair("two", "bite")));
	}
}


TEST(FlatMapTest, Brackets)
{
	{
		FlatMap testee;

		std::string one("one");
		testee[std::move(one)] = "jaws";
		EXPECT_TRUE(one.empty());

		std::string two("two");
		testee[std::move(two)] = "bite";
		EXPECT_TRUE(two.empty());

		std::string three("three");
		testee[std::move(three)] = "claws";
		EXPECT_TRUE(three.empty());

		std::string four("four");
		testee[std::move(four)] = "catch";
		EXPECT_TRUE(four.empty());

		std::string four2("four");
		testee[std::move(four2)] = "dup";
		EXPECT_FALSE(four2.empty());

		std::string three2("three");
		testee[std::move(three2)] = "dup";
		EXPECT_FALSE(three2.empty());

		EXPECT_TRUE(std::is_sorted(testee.begin(), testee.end(), testee.value_comp()));

		ASSERT_THAT(testee, ElementsAre(std::make_pair("four", "dup"), std::make_pair("one", "jaws"), std::make_pair("three", "dup"), std::make_pair("two", "bite")));
	}
}


TEST(FlatMapTest, Insertion)
{
	{
		FlatMap testee;
		EXPECT_TRUE(testee.insert(std::make_pair("one", "jaws")).second);
		EXPECT_TRUE(testee.insert(std::make_pair("two", "bite")).second);
		EXPECT_TRUE(testee.insert(std::make_pair("three", "claws")).second);
		EXPECT_TRUE(testee.insert(std::make_pair("four", "catch")).second);
		EXPECT_FALSE(testee.insert(std::make_pair("four", "dup")).second);
		EXPECT_FALSE(testee.insert(std::make_pair("three", "dup")).second);

		EXPECT_TRUE(std::is_sorted(testee.begin(), testee.end(), testee.value_comp()));

		ASSERT_THAT(testee, ElementsAre(std::make_pair("four", "catch"), std::make_pair("one", "jaws"), std::make_pair("three", "claws"), std::make_pair("two", "bite")));
	}
	{
		FlatMap testee;

		FlatMap::value_type one("one", "jaws");
		EXPECT_TRUE(testee.insert(std::move(one)).second);
		EXPECT_TRUE(one.first.empty());
		EXPECT_TRUE(one.second.empty());

		FlatMap::value_type two("two", "bite");
		EXPECT_TRUE(testee.insert(std::move(two)).second);
		EXPECT_TRUE(two.first.empty());
		EXPECT_TRUE(two.second.empty());

		FlatMap::value_type three("three", "claws");
		EXPECT_TRUE(testee.insert(std::move(three)).second);
		EXPECT_TRUE(three.first.empty());
		EXPECT_TRUE(three.second.empty());

		FlatMap::value_type four("four", "catch");
		EXPECT_TRUE(testee.insert(std::move(four)).second);
		EXPECT_TRUE(four.first.empty());
		EXPECT_TRUE(four.second.empty());

		FlatMap::value_type four2("four", "dup");
		EXPECT_FALSE(testee.insert(std::move(four2)).second);
		EXPECT_FALSE(four2.first.empty());
		EXPECT_FALSE(four2.second.empty());

		FlatMap::value_type three2("three", "dup");
		EXPECT_FALSE(testee.insert(std::move(three2)).second);
		EXPECT_FALSE(three2.first.empty());
		EXPECT_FALSE(three2.second.empty());

		EXPECT_TRUE(std::is_sorted(testee.begin(), testee.end(), testee.value_comp()));

		ASSERT_THAT(testee, ElementsAre(std::make_pair("four", "catch"), std::make_pair("one", "jaws"), std::make_pair("three", "claws"), std::make_pair("two", "bite")));
	}
	{
		FlatMap testee;

		FlatMap::value_type one("one", "jaws");
		testee.insert(testee.end(), std::move(one));
		EXPECT_TRUE(one.first.empty());
		EXPECT_TRUE(one.second.empty());

		FlatMap::value_type two("two", "bite");
		testee.insert(testee.end(), std::move(two));
		EXPECT_TRUE(two.first.empty());
		EXPECT_TRUE(two.second.empty());

		FlatMap::value_type three("three", "claws");
		testee.insert(testee.begin(), std::move(three));
		EXPECT_TRUE(three.first.empty());
		EXPECT_TRUE(three.second.empty());

		FlatMap::value_type four("four", "catch");
		testee.insert(testee.begin(), std::move(four));
		EXPECT_TRUE(four.first.empty());
		EXPECT_TRUE(four.second.empty());

		FlatMap::value_type four2("four", "dup");
		testee.insert(testee.end(), std::move(four2));
		EXPECT_FALSE(four2.first.empty());
		EXPECT_FALSE(four2.second.empty());

		FlatMap::value_type three2("three", "dup");
		testee.insert(testee.end(), std::move(three2));
		EXPECT_FALSE(three2.first.empty());
		EXPECT_FALSE(three2.second.empty());

		EXPECT_TRUE(std::is_sorted(testee.begin(), testee.end(), testee.value_comp()));

		ASSERT_THAT(testee, ElementsAre(std::make_pair("four", "catch"), std::make_pair("one", "jaws"), std::make_pair("three", "claws"), std::make_pair("two", "bite")));
	}
}


TEST(FlatMapTest, Emplacing)
{
	{
		FlatMap testee;
		EXPECT_TRUE(testee.emplace(std::make_pair("one", "jaws")).second);
		EXPECT_TRUE(testee.emplace(std::make_pair("two", "bite")).second);
		EXPECT_TRUE(testee.emplace(std::make_pair("three", "claws")).second);
		EXPECT_TRUE(testee.emplace(std::make_pair("four", "catch")).second);
		EXPECT_FALSE(testee.emplace(std::make_pair("four", "dup")).second);
		EXPECT_FALSE(testee.emplace(std::make_pair("three", "dup")).second);

		EXPECT_TRUE(std::is_sorted(testee.begin(), testee.end(), testee.value_comp()));

		ASSERT_THAT(testee, ElementsAre(std::make_pair("four", "catch"), std::make_pair("one", "jaws"), std::make_pair("three", "claws"), std::make_pair("two", "bite")));
	}
	{
		FlatMap testee;

		testee.emplace_hint(testee.end(), "one", "jaws");
		testee.emplace_hint(testee.end(), "two", "bite");
		testee.emplace_hint(testee.begin(), "three", "claws");
		testee.emplace_hint(testee.begin(), "four", "catch");
		testee.emplace_hint(testee.end(), "four", "dup");
		testee.emplace_hint(testee.end(), "three", "dup");

		EXPECT_TRUE(std::is_sorted(testee.begin(), testee.end(), testee.value_comp()));

		ASSERT_THAT(testee, ElementsAre(std::make_pair("four", "catch"), std::make_pair("one", "jaws"), std::make_pair("three", "claws"), std::make_pair("two", "bite")));
	}
}


TEST(FlatMapTest, Lookup)
{
	Vector unordered = GetUnorderedVector();

	FlatMap testee;
	Map sample;

	testee.reserve(unordered.size());
	for (Vector::const_iterator it = unordered.begin(); it != unordered.end(); ++it)
	{
		testee.insert(*it);
		sample.insert(*it);
	}

	EXPECT_TRUE(std::is_sorted(testee.begin(), testee.end(), testee.value_comp()));
	EXPECT_TRUE(std::equal(sample.begin(), sample.end(), testee.begin(), testee.end(), PairEquals()));

	for (Map::const_iterator sample_iter = sample.begin(); sample_iter != sample.end(); ++sample_iter)
	{
		FlatMap::const_iterator testee_iter = testee.find(sample_iter->first);
		ASSERT_NE(testee_iter, testee.end());
		EXPECT_TRUE(PairEquals()(*testee_iter, *sample_iter));
	}
	for (FlatMap::const_iterator testee_iter = testee.begin(); testee_iter != testee.end(); ++testee_iter)
	{
		Map::const_iterator sample_iter = sample.find(testee_iter->first);
		ASSERT_NE(sample_iter, sample.end());
		EXPECT_TRUE(PairEquals()(*sample_iter, *testee_iter));
	}

	for (Map::const_reverse_iterator sample_iter = sample.rbegin(); sample_iter != sample.rend(); ++sample_iter)
	{
		FlatMap::const_iterator testee_iter = testee.find(sample_iter->first);
		ASSERT_NE(testee_iter, testee.end());
		EXPECT_TRUE(PairEquals()(*testee_iter, *sample_iter));
	}

	for (FlatMap::const_reverse_iterator testee_iter = testee.rbegin(); testee_iter != testee.rend(); ++testee_iter)
	{
		Map::const_iterator sample_iter = sample.find(testee_iter->first);
		ASSERT_NE(sample_iter, sample.end());
		EXPECT_TRUE(PairEquals()(*sample_iter, *testee_iter));
	}

	for (Map::const_iterator sample_iter = sample.begin(); sample_iter != sample.end(); ++sample_iter)
		ASSERT_NO_THROW(EXPECT_EQ(testee.at(sample_iter->first), sample_iter->second));
	for (FlatMap::const_iterator testee_iter = testee.begin(); testee_iter != testee.end(); ++testee_iter)
		ASSERT_NO_THROW(EXPECT_EQ(sample.at(testee_iter->first), testee_iter->second));

	for (Map::const_iterator sample_iter = sample.begin(); sample_iter != sample.end(); ++sample_iter)
	{
		ASSERT_TRUE(testee.count(sample_iter->first));
		EXPECT_EQ(testee[sample_iter->first], sample_iter->second);
	}
	for (FlatMap::const_iterator testee_iter = testee.begin(); testee_iter != testee.end(); ++testee_iter)
	{
		ASSERT_TRUE(sample.count(testee_iter->first));
		EXPECT_EQ(sample[testee_iter->first], testee_iter->second);
	}
}


TEST(FlatMapTest, TransparentLookup)
{
	Vector unordered = GetUnorderedVector();

	TransparentFlatMap testee;
	Map sample;

	testee.reserve(unordered.size());
	for (Vector::const_iterator it = unordered.begin(); it != unordered.end(); ++it)
	{
		testee.insert(*it);
		sample.insert(*it);
	}

	EXPECT_TRUE(std::is_sorted(testee.begin(), testee.end(), testee.value_comp()));
	EXPECT_TRUE(std::equal(sample.begin(), sample.end(), testee.begin(), testee.end(), PairEquals()));

	for (Map::const_iterator sample_iter = sample.begin(); sample_iter != sample.end(); ++sample_iter)
	{
		TransparentFlatMap::const_iterator testee_iter = testee.find(string_view(sample_iter->first));
		ASSERT_NE(testee_iter, testee.end());
		EXPECT_TRUE(PairEquals()(*testee_iter, *sample_iter));
	}

	for (Map::const_reverse_iterator sample_iter = sample.rbegin(); sample_iter != sample.rend(); ++sample_iter)
	{
		TransparentFlatMap::const_iterator testee_iter = testee.find(string_view(sample_iter->first));
		ASSERT_NE(testee_iter, testee.end());
		EXPECT_TRUE(PairEquals()(*testee_iter, *sample_iter));
	}

	for (Map::const_iterator sample_iter = sample.begin(); sample_iter != sample.end(); ++sample_iter)
		ASSERT_NO_THROW(EXPECT_EQ(testee.at(string_view(sample_iter->first)), sample_iter->second));

	for (Map::const_iterator sample_iter = sample.begin(); sample_iter != sample.end(); ++sample_iter)
	{
		ASSERT_TRUE(testee.count(string_view(sample_iter->first)));
		EXPECT_EQ(testee[sample_iter->first], sample_iter->second);
	}
}


TEST(FlatMapTest, BoundsLookup)
{
	const FlatMap testee = { { "1", "1" }, { "3", "3" }, { "5", "5" }, { "7", "7" } };

	{
		const FlatMap::const_iterator iter = testee.lower_bound("3");
		ASSERT_NE(iter, testee.end());
		ASSERT_EQ(iter->first, "3");
	}

	{
		const FlatMap::const_iterator iter = testee.lower_bound("4");
		ASSERT_NE(iter, testee.end());
		ASSERT_EQ(iter->first, "5");
	}

	{
		const FlatMap::const_iterator iter = testee.lower_bound("5");
		ASSERT_NE(iter, testee.end());
		ASSERT_EQ(iter->first, "5");
	}

	{
		const FlatMap::const_iterator iter = testee.lower_bound("8");
		ASSERT_EQ(iter, testee.end());
	}

	{
		const FlatMap::const_iterator iter = testee.upper_bound("3");
		ASSERT_NE(iter, testee.end());
		ASSERT_EQ(iter->first, "5");
	}

	{
		const FlatMap::const_iterator iter = testee.upper_bound("4");
		ASSERT_NE(iter, testee.end());
		ASSERT_EQ(iter->first, "5");
	}

	{
		const FlatMap::const_iterator iter = testee.upper_bound("5");
		ASSERT_NE(iter, testee.end());
		ASSERT_EQ(iter->first, "7");
	}

	{
		const FlatMap::const_iterator iter = testee.upper_bound("8");
		ASSERT_EQ(iter, testee.end());
	}

	{
		const auto iterPair = testee.equal_range("3");
		ASSERT_NE(iterPair.first, iterPair.second);
		ASSERT_EQ(iterPair.first->first, "3");
		ASSERT_EQ(iterPair.second->first, "5");
	}

	{
		const auto iterPair = testee.equal_range("4");
		ASSERT_EQ(iterPair.first, iterPair.second);
	}

	{
		const auto iterPair = testee.equal_range("5");
		ASSERT_NE(iterPair.first, iterPair.second);
		ASSERT_EQ(iterPair.first->first, "5");
		ASSERT_EQ(iterPair.second->first, "7");
	}

	{
		const auto iterPair = testee.equal_range("8");
		ASSERT_EQ(iterPair.first, iterPair.second);
	}
}


TEST(FlatMapTest, TransparentBoundsLookup)
{
	const TransparentFlatMap testee = { { "1", "1" }, { "3", "3" }, { "5", "5" }, { "7", "7" } };

	{
		const TransparentFlatMap::const_iterator iter = testee.lower_bound(string_view("3"));
		ASSERT_NE(iter, testee.end());
		ASSERT_EQ(iter->first, "3");
	}

	{
		const TransparentFlatMap::const_iterator iter = testee.lower_bound(string_view("4"));
		ASSERT_NE(iter, testee.end());
		ASSERT_EQ(iter->first, "5");
	}

	{
		const TransparentFlatMap::const_iterator iter = testee.lower_bound(string_view("5"));
		ASSERT_NE(iter, testee.end());
		ASSERT_EQ(iter->first, "5");
	}

	{
		const TransparentFlatMap::const_iterator iter = testee.lower_bound(string_view("8"));
		ASSERT_EQ(iter, testee.end());
	}

	{
		const TransparentFlatMap::const_iterator iter = testee.upper_bound(string_view("3"));
		ASSERT_NE(iter, testee.end());
		ASSERT_EQ(iter->first, "5");
	}

	{
		const TransparentFlatMap::const_iterator iter = testee.upper_bound(string_view("4"));
		ASSERT_NE(iter, testee.end());
		ASSERT_EQ(iter->first, "5");
	}

	{
		const TransparentFlatMap::const_iterator iter = testee.upper_bound(string_view("5"));
		ASSERT_NE(iter, testee.end());
		ASSERT_EQ(iter->first, "7");
	}

	{
		const TransparentFlatMap::const_iterator iter = testee.upper_bound(string_view("8"));
		ASSERT_EQ(iter, testee.end());
	}

	{
		const auto iterPair = testee.equal_range(string_view("3"));
		ASSERT_NE(iterPair.first, iterPair.second);
		ASSERT_EQ(iterPair.first->first, "3");
		ASSERT_EQ(iterPair.second->first, "5");
	}

	{
		const auto iterPair = testee.equal_range(string_view("4"));
		ASSERT_EQ(iterPair.first, iterPair.second);
	}

	{
		const auto iterPair = testee.equal_range(string_view("5"));
		ASSERT_NE(iterPair.first, iterPair.second);
		ASSERT_EQ(iterPair.first->first, "5");
		ASSERT_EQ(iterPair.second->first, "7");
	}

	{
		const auto iterPair = testee.equal_range(string_view("8"));
		ASSERT_EQ(iterPair.first, iterPair.second);
	}
}


TEST(FlatMapTest, Removal)
{
	{
		FlatMap testee(GetSampleFlatMap());
		ASSERT_TRUE(testee.count("Ukrainians"));
		ASSERT_TRUE(testee.erase("Ukrainians"));
		ASSERT_FALSE(testee.erase("Ukrainians"));
	}
	{
		FlatMap testee(GetSampleFlatMap());
		FlatMap::const_iterator first = testee.find("Russians");
		ASSERT_TRUE(first != testee.end());
		FlatMap::const_iterator next = testee.erase(first);
		ASSERT_EQ(next->first, "Ukrainians");
		ASSERT_FALSE(testee.count("Russians"));
	}
	{
		FlatMap testee(GetSampleFlatMap());
		FlatMap::const_iterator first = testee.find("Americans");
		FlatMap::const_iterator last = testee.find("Japaneses");
		ASSERT_TRUE(first != testee.end());
		ASSERT_TRUE(last != testee.end());
		FlatMap::const_iterator next = testee.erase(first, last);
		ASSERT_EQ(next->first, "Japaneses");
		ASSERT_FALSE(testee.erase("Americans"));
		ASSERT_TRUE(testee.count("Japaneses"));
	}
}
