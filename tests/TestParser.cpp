#include <parser.h>

#include <gtest/gtest.h>

TEST(TestParser, emptyString)
{
    double value{-1.0};

    ASSERT_FALSE(fractalFormula::parse("", value));
}

TEST(TestParser, realNumber)
{
    double value{-1.0};

    ASSERT_TRUE(fractalFormula::parse("3.14159", value));
    ASSERT_EQ(3.14159, value);
}
