#include <Complex.h>

#include <gtest/gtest.h>

using namespace fractalFormula;

TEST(TestComplex, equal)
{
    const Complex lhs{1.0, 2.0};
    const Complex rhs{1.0, 2.0};

    ASSERT_EQ(lhs, rhs);
}

TEST(TestComplex, notEqual)
{
    const Complex lhs{1.0, 2.0};
    const Complex rhs{2.0, 1.0};

    ASSERT_NE(lhs, rhs);
}
