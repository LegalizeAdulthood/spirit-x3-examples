#include <parser.h>

#include <gtest/gtest.h>

TEST(TestParser, basic)
{
    ASSERT_TRUE(fractalFormula::parse(""));
}
