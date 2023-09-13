#include <FormulaParser.h>
#include <ASTPrinter.h>

#include <gtest/gtest.h>

#include <sstream>

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

TEST(TestFormulaParser, i)
{
    ast::Expression value;

    const bool parsed = parseFormula("i", value);

    ASSERT_TRUE(parsed);
    const auto &var = value.first.get();
    ASSERT_FALSE(var.empty());
    const auto &expr = boost::get<x3::forward_ast<ast::Expression>>(var).get();
    ASSERT_EQ(1.0, boost::get<x3::forward_ast<ast::Imaginary>>(expr.first.get()).get().value);
}

TEST(TestFormulaParser, imaginary)
{
    ast::Expression value;

    const bool parsed = parseFormula("3.14159i", value);

    ASSERT_TRUE(parsed);
    const auto &var = value.first.get();
    ASSERT_FALSE(var.empty());
    const auto &expr = boost::get<x3::forward_ast<ast::Expression>>(var).get();
    ASSERT_EQ(3.14159, boost::get<x3::forward_ast<ast::Imaginary>>(expr.first.get()).get().value);
}

TEST(TestFormulaParser, notANumber)
{
    ast::Expression value;

    const bool parsed = parseFormula("3.9f17", value);

    ASSERT_FALSE(parsed);
}

TEST(TestFormulaParser, add)
{
    ast::Expression value;

    const bool parsed = parseFormula("3 + 4", value);

    ASSERT_TRUE(parsed);
    const ast::Operand::variant_type &var = value.first.get();
    ASSERT_FALSE(var.empty());
    const ast::Expression &expr = boost::get<x3::forward_ast<ast::Expression>>(var).get();
    ASSERT_EQ(3.0, boost::get<double>(expr.first.get()));
    const std::vector<ast::Operation> &rest = value.rest;
    ASSERT_EQ(1U, rest.size());
    ASSERT_EQ('+', rest[0].operation);
    const ast::Expression &rhsExpr = boost::get<x3::forward_ast<ast::Expression>>(rest[0].operand).get();
    ASSERT_EQ(4.0, boost::get<double>(rhsExpr.first.get()));
}

TEST(TestASTPrinter, add)
{
    std::ostringstream str;
    ast::Expression value;
    const bool parsed = parseFormula("3+4", value);

    ast::Printer printer(str);
    printer(value);

    ASSERT_EQ("(3 + 4)", str.str());
}
