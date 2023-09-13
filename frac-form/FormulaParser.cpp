#include <FormulaParser.h>
#include <AST.h>
#include <ASTAdapted.h>

#include <boost/fusion/adapted.hpp>
#include <boost/spirit/home/x3.hpp>

namespace x3 = boost::spirit::x3;
namespace ascii = x3::ascii;

namespace fractalFormula
{

struct AdditiveExpr_tag;
using AdditiveExprRule = x3::rule<AdditiveExpr_tag, ast::Expression>;
AdditiveExprRule const AdditiveExpr = "additive_expr";

struct MultiplicativeExpr_tag;
using MultiplicativeExprRule = x3::rule<MultiplicativeExpr_tag, ast::Expression>;
MultiplicativeExprRule const MultiplicativeExpr = "multiplicative_expr";

struct UnaryExpr_tag;
using UnaryExprRule = x3::rule<UnaryExpr_tag, ast::Operand>;
UnaryExprRule const UnaryExpr = "unary_expr";

struct PrimaryExpr_tag;
using PrimaryExprRule = x3::rule<PrimaryExpr_tag, ast::Operand>;
PrimaryExprRule const PrimaryExpr = "primary_expr";

struct ArgumentList_tag;
using ArgumentListRule = x3::rule<ArgumentList_tag, std::vector<ast::Expression>>;
ArgumentListRule const ArgumentList = "argument_list";

struct Identifier_tag;
using IdentifierRule = x3::rule<Identifier_tag, std::string>;
IdentifierRule const Identifier = "identifier";

struct FunctionCall_tag;
using FunctionCallRule = x3::rule<FunctionCall_tag, ast::FunctionCall>;
FunctionCallRule const FunctionCall = "function_call";

struct Expression_tag;
using ExpressionRule = x3::rule<Expression_tag, ast::Expression>;
ExpressionRule const Expression = "expression";

// clang-format off
auto const AdditiveExpr_def =
    MultiplicativeExpr >>
    *(
        (x3::char_('+') > MultiplicativeExpr)
        | (x3::char_('-') > MultiplicativeExpr)
    );

auto const MultiplicativeExpr_def =
    UnaryExpr >>
    *(
        (x3::char_('*') > UnaryExpr)
        | (x3::char_('/') > UnaryExpr)
    );

auto const UnaryExpr_def =
    PrimaryExpr
    | (x3::char_('-') > PrimaryExpr)
    | (x3::char_('-') > PrimaryExpr);

auto const ArgumentList_def = Expression % ',';

auto const Identifier_def =
    x3::raw[x3::lexeme[(x3::alpha | '_') >> *(x3::alnum | '_')]];

auto const FunctionCall_def =
    Identifier >> -('(' > ArgumentList > ')');

auto const PrimaryExpr_def =
    x3::double_
    | FunctionCall
    | '(' > Expression > ')';

auto const Expression_def = AdditiveExpr;
// clang-format on

BOOST_SPIRIT_DEFINE(AdditiveExpr, MultiplicativeExpr, UnaryExpr, ArgumentList, Identifier, FunctionCall, PrimaryExpr,
                    Expression);

bool parseFormula(const std::string &text, ast::Expression &value)
{
    using ascii::space;
    using x3::double_;
    using x3::phrase_parse;

    using Iterator = std::string::const_iterator;
    Iterator       begin = text.begin();
    const Iterator end = text.end();
    const bool     result = phrase_parse(begin, end, Expression, space, value) && begin == end;
    return result;
}

} // namespace fractalFormula
