// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/math.h>
#include <stingraykit/log/Logger.h>

#include <gmock/gmock-matchers.h>

using ::testing::Field;

using namespace stingray;

#define CHECK_REMAINDER(Dividend, Divisor, TargetPrecision, ResultRemainder, ResultPrecision, ResultIsOverflow) \
		ASSERT_THAT(CalculateFractionRemainder(Dividend, Divisor, TargetPrecision), \
				AllOf( \
						Field(&FractionInfo::Fraction, ResultRemainder), \
						Field(&FractionInfo::Precision, ResultPrecision), \
						Field(&FractionInfo::IsOverflow, ResultIsOverflow)))

TEST(MathTest, FractionRemainder)
{
	CHECK_REMAINDER(18446744073709551615ull, 18446744073709551615ull, 20, 0, 0, false);

	CHECK_REMAINDER(18446744073709551615ull, 1024ull, 20, 9990234375000000, 16, false); // 18,014,398,509,481,983.9990'2343'75
	CHECK_REMAINDER(18446744073709551615ull, 1024ull, 8, 99902344, 8, false);
	CHECK_REMAINDER(18446744073709551615ull, 1024ull, 3, 999, 3, false);
	CHECK_REMAINDER(18446744073709551615ull, 1024ull, 2, 0, 0, true);
	CHECK_REMAINDER(18446744073709551615ull, 1024ull, 0, 0, 0, true);

	CHECK_REMAINDER(18446744073709551615ull, 1024ull * 1024, 20, 9999990463256, 13, false); // 17,592,186,044,415.9999'9904'6325'6835'9375
	CHECK_REMAINDER(18446744073709551615ull, 1024ull * 1024, 8, 99999905, 8, false);
	CHECK_REMAINDER(18446744073709551615ull, 1024ull * 1024, 6, 999999, 6, false);
	CHECK_REMAINDER(18446744073709551615ull, 1024ull * 1024, 4, 0, 0, true);

	CHECK_REMAINDER(18446744073709551615ull, 1024ull * 1024 * 1024, 20, 9999999990, 10, false); // 17,179,869,183.9999'9999'9068'6774'2538
	CHECK_REMAINDER(18446744073709551615ull, 1024ull * 1024 * 1024, 9, 999999999, 9, false);
	CHECK_REMAINDER(18446744073709551615ull, 1024ull * 1024 * 1024, 8, 0, 0, true);
	CHECK_REMAINDER(18446744073709551615ull, 1024ull * 1024 * 1024, 0, 0, 0, true);

	CHECK_REMAINDER(18446744073709551615ull, 1024ull * 1024 * 1024 * 1024, 20, 9999999, 7, false); // 16,777,215.9999'9999'9999'0905'0529
	CHECK_REMAINDER(18446744073709551615ull, 1024ull * 1024 * 1024 * 1024, 6, 0, 0, true);
	CHECK_REMAINDER(18446744073709551615ull, 1024ull * 1024 * 1024 * 1024, 1, 0, 0, true);
	CHECK_REMAINDER(18446744073709551615ull, 1024ull * 1024 * 1024 * 1024, 0, 0, 0, true);

	CHECK_REMAINDER(18446744073709551615ull, 1024ull * 1024 * 1024 * 1024 * 1024, 20, 9999, 4, false); // 16,383.9999'9999'9999'9991'1182
	CHECK_REMAINDER(18446744073709551615ull, 1024ull * 1024 * 1024 * 1024 * 1024, 3, 0, 0, true);

	CHECK_REMAINDER(18446744073709551615ull, 179999999999999999ull, 20, 48, 2, false); // 102.4819'1152'0608'6206'5267
	CHECK_REMAINDER(18446744073709551615ull, 179999999999999999ull, 1, 5, 1, false);
	CHECK_REMAINDER(18446744073709551615ull, 179999999999999999ull, 0, 0, 0, true);

	CHECK_REMAINDER(18446744073709551615ull, 159999999999999999ull, 20, 29, 2, false); // 115.2921'5046'0684'6983'1432
	CHECK_REMAINDER(18446744073709551615ull, 159999999999999999ull, 1, 3, 1, false);
	CHECK_REMAINDER(18446744073709551615ull, 159999999999999999ull, 0, 0, 0, false);

	CHECK_REMAINDER(18446744073709551615ull, 139999999999999999ull, 20, 76, 2, false); // 131.7624'5766'9353'9410'483
	CHECK_REMAINDER(18446744073709551615ull, 139999999999999999ull, 1, 8, 1, false);
	CHECK_REMAINDER(18446744073709551615ull, 139999999999999999ull, 0, 0, 0, true);

	CHECK_REMAINDER(18446744073709551615ull, 119999999999999999ull, 20, 72, 2, false); // 153.7228'6728'0912'9314'0602
	CHECK_REMAINDER(18446744073709551615ull, 119999999999999999ull, 1, 7, 1, false);
	CHECK_REMAINDER(18446744073709551615ull, 119999999999999999ull, 0, 0, 0, true);

	CHECK_REMAINDER(18446744073709551615ull, 99999999999999999ull, 20, 46, 2, false); // 184.4674'4073'7095'5179'9467
	CHECK_REMAINDER(18446744073709551615ull, 99999999999999999ull, 1, 5, 1, false);
	CHECK_REMAINDER(18446744073709551615ull, 99999999999999999ull, 0, 0, 0, true);

	CHECK_REMAINDER(18446744073709551615ull, 79999999999999999ull, 20, 58, 2, false); // 230.5843'0092'1369'3980'698
	CHECK_REMAINDER(18446744073709551615ull, 79999999999999999ull, 1, 6, 1, false);
	CHECK_REMAINDER(18446744073709551615ull, 79999999999999999ull, 0, 0, 0, true);

	CHECK_REMAINDER(18446744073709551615ull, 59999999999999999ull, 20, 44, 2, false); // 307.4457'3456'1825'8653'7409
	CHECK_REMAINDER(18446744073709551615ull, 59999999999999999ull, 1, 4, 1, false);
	CHECK_REMAINDER(18446744073709551615ull, 59999999999999999ull, 0, 0, 0, false);

	CHECK_REMAINDER(18446744073709551615ull, 39999999999999999ull, 20, 168, 3, false); // 461.1686'0184'2738'8019'0421
	CHECK_REMAINDER(18446744073709551615ull, 39999999999999999ull, 2, 17, 2, false);
	CHECK_REMAINDER(18446744073709551615ull, 39999999999999999ull, 1, 2, 1, false);
	CHECK_REMAINDER(18446744073709551615ull, 39999999999999999ull, 0, 0, 0, false);

	CHECK_REMAINDER(18446744073709551615ull, 19999999999999999ull, 20, 337, 3, false); // 922.3372'0368'5477'6268'6686
	CHECK_REMAINDER(18446744073709551615ull, 19999999999999999ull, 2, 34, 2, false);
	CHECK_REMAINDER(18446744073709551615ull, 19999999999999999ull, 1, 3, 1, false);
	CHECK_REMAINDER(18446744073709551615ull, 19999999999999999ull, 0, 0, 0, false);

	CHECK_REMAINDER(2038897ull, 1048576ull, 20, 9444437026977, 13, false); // 1.9444'4370'2697'7539'0625
	CHECK_REMAINDER(2038897ull, 1048576ull, 12, 944443702698, 12, false);
	CHECK_REMAINDER(2038897ull, 1048576ull, 11, 94444370270, 11, false);
	CHECK_REMAINDER(2038897ull, 1048576ull, 9, 944443703, 9, false);
	CHECK_REMAINDER(2038897ull, 1048576ull, 6, 944444, 6, false);
	CHECK_REMAINDER(2038897ull, 1048576ull, 1, 9, 1, false);
	CHECK_REMAINDER(2038897ull, 1048576ull, 0, 0, 0, true);

	CHECK_REMAINDER(2038898ull, 1048576ull, 20, 9444446563720, 13, false); // 1.9444'4465'6372'0703'125
	CHECK_REMAINDER(2038898ull, 1048576ull, 11, 94444465637, 11, false);
	CHECK_REMAINDER(2038898ull, 1048576ull, 10, 9444446564, 10, false);
	CHECK_REMAINDER(2038898ull, 1048576ull, 6, 944445, 6, false);
	CHECK_REMAINDER(2038898ull, 1048576ull, 2, 95, 2, false);
	CHECK_REMAINDER(2038898ull, 1048576ull, 1, 0, 0, true);
	CHECK_REMAINDER(2038898ull, 1048576ull, 0, 0, 0, true);
}


#define CHECK_FRACTION(FractionStr, TargetPrecision, ResultFraction, ResultPrecision, ResultIsOverflow) \
		ASSERT_THAT(ParseDecimalFraction(FractionStr, TargetPrecision), \
					AllOf( \
						   Field(&FractionInfo::Fraction, ResultFraction), \
						   Field(&FractionInfo::Precision, ResultPrecision), \
						   Field(&FractionInfo::IsOverflow, ResultIsOverflow)))

TEST(MathTest, DecimalFraction)
{
	ASSERT_ANY_THROW(ParseDecimalFraction("18446744073709551616", 20));

	CHECK_FRACTION("18446744073709551615", 20, 18446744073709551615ull, 20, false);
	ASSERT_ANY_THROW(ParseDecimalFraction("18446744073709551615", 21));
	CHECK_FRACTION("18446744073709551615", 3, 185, 3, false);
	CHECK_FRACTION("18446744073709551615", 2, 19, 2, false);
	CHECK_FRACTION("18446744073709551615", 1, 2, 1, false);

	CHECK_FRACTION("18446744073709551614", 20, 18446744073709551614ull, 20, false);
	ASSERT_ANY_THROW(ParseDecimalFraction("18446744073709551614", 21));

	CHECK_FRACTION("1844674407370955162", 19, 1844674407370955162ull, 19, false);
	ASSERT_ANY_THROW(ParseDecimalFraction("1844674407370955162", 20));
	CHECK_FRACTION("1844674407370955161", 20, 18446744073709551610ull, 20, false);

	CHECK_FRACTION("87654", 6, 876540, 6, false);
	CHECK_FRACTION("87654", 5, 87654, 5, false);
	CHECK_FRACTION("87654", 4, 8765, 4, false);
	CHECK_FRACTION("87654", 3, 877, 3, false);
	CHECK_FRACTION("87654", 2, 88, 2, false);
	CHECK_FRACTION("87654", 1, 9, 1, false);
	CHECK_FRACTION("87654", 0, 0, 0, true);

	CHECK_FRACTION("987654", 8, 98765400, 8, false);
	CHECK_FRACTION("987654", 7, 9876540, 7, false);
	CHECK_FRACTION("987654", 6, 987654, 6, false);
	CHECK_FRACTION("987654", 5, 98765, 5, false);
	CHECK_FRACTION("987654", 4, 9877, 4, false);
	CHECK_FRACTION("987654", 3, 988, 3, false);
	CHECK_FRACTION("987654", 2, 99, 2, false);
	CHECK_FRACTION("987654", 1, 0, 0, true);
	CHECK_FRACTION("987654", 0, 0, 0, true);

	CHECK_FRACTION("000000000000000000000000", 30, 0, 0, false);
	CHECK_FRACTION("0", 20, 0, 0, false);
}
