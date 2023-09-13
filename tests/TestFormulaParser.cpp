#include <FormulaParser.h>

#include <gtest/gtest.h>

using namespace fractalFormula;
namespace x3 = boost::spirit::x3;

TEST(TestFormulaParser, emptyString)
{
    ast::Expression value;

    ASSERT_FALSE(parseFormula("", value));
}

TEST(TestFormulaParser, realNumber)
{
    ast::Expression value;
    const double expected{3.14159};

    const bool parsed = parseFormula("3.14159", value);

    ASSERT_TRUE(parsed);
    const auto &var = value.first.get();
    ASSERT_FALSE(var.empty());
    const auto &expr = boost::get<x3::forward_ast<ast::Expression>>(var).get();
    ASSERT_EQ(expected, boost::get<double>(expr.first.get()));
}

TEST(TestFormulaParser, notANumber)
{
    ast::Expression value;

    const bool parsed = parseFormula("3.9f17", value);

    ASSERT_FALSE(parsed);
}
