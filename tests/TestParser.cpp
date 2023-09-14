#include <Parser.h>

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
    const Complex expected{3.14159, 0.0};

    const bool parsed = parse("3.14159", value);

    ASSERT_TRUE(parsed);
    ASSERT_EQ(expected, value);
}

TEST(TestParser, i)
{
    Complex value{-1.0, -1.0};
    const Complex expected{0.0, 1.0};

    const bool parsed = parse("i", value);

    ASSERT_TRUE(parsed);
    ASSERT_EQ(expected, value);
}

TEST(TestParser, imaginary)
{
    Complex value{-1.0, -1.0};
    const Complex expected{0.0, 5.0};

    const bool parsed = parse("5.0 i", value);

    ASSERT_TRUE(parsed);
    ASSERT_EQ(expected, value);
}
