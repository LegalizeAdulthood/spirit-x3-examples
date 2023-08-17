#include <parser.h>

#include <gtest/gtest.h>

using namespace fractalFormula;

TEST(TestParser, emptyString)
{
    Complex value{-1.0, -1.0};

    ASSERT_FALSE(parse("", value));
}

TEST(TestParser, realNumber)
{
    Complex value{-1.0, -1.0};

    ASSERT_TRUE(parse("3.14159", value));
    ASSERT_EQ(3.14159, value.re);
    ASSERT_EQ(0.0, value.im);
}
