#include <AST.h>
#include <ASTAdapted.h>
#include <FormulaParser.h>

#include <boost/fusion/adapted.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/utility/lambda_visitor.hpp>

#include <map>

namespace x3 = boost::spirit::x3;
namespace ascii = x3::ascii;

namespace fractalFormula
{

struct ErrorHandlerTag;

template <typename Iterator>
using ErrorHandler = x3::error_handler<Iterator>;

struct Annotation
{
    template <typename Iterator, typename Context>
    void on_success(Iterator const &first, Iterator const &last, ast::Operand &ast, Context const &context);

    template <typename T, typename Iterator, typename Context>
    void on_success(Iterator const &first, Iterator const &last, T &ast, Context const &context);
};

template <typename Iterator, typename Context>
void Annotation::on_success(Iterator const &first, Iterator const &last, ast::Operand &ast, Context const &context)
{
    auto &errorHandler = x3::get<ErrorHandlerTag>(context).get();
    auto  annotate = [&](auto &node) { errorHandler.tag(node, first, last); };
    ast.apply_visitor(x3::make_lambda_visitor<void>(annotate));
}

template <typename T, typename Iterator, typename Context>
void Annotation::on_success(Iterator const &first, Iterator const &last, T &ast, Context const &context)
{
    auto &errorHandler = x3::get<ErrorHandlerTag>(context).get();
    errorHandler.tag(ast, first, last);
}

struct OnFailure
{
    OnFailure();

    template <typename Iterator, typename Exception, typename Context>
    x3::error_handler_result on_error(Iterator &first, const Iterator &last, Exception const &bang,
                                      Context const &context)
    {
        std::string which = bang.which();
        if (auto iter = m_ruleNameToExplanation.find(which); iter != m_ruleNameToExplanation.end())
        {
            which = iter->second;
        }

        std::string message = "Error! Expecting " + which + " here:";
        auto       &errorHandler = x3::get<ErrorHandlerTag>(context).get();
        errorHandler(bang.where(), message);
        return x3::error_handler_result::fail;
    }

    std::map<std::string, std::string> m_ruleNameToExplanation;
};

OnFailure::OnFailure()
{
    m_ruleNameToExplanation["additive_expr"] = "Expression";
    m_ruleNameToExplanation["multiplicative_expr"] = "Expression";
    m_ruleNameToExplanation["unary_expr"] = "Expression";
    m_ruleNameToExplanation["expression"] = "Expression";
    m_ruleNameToExplanation["function_call"] = "Function Call";
    m_ruleNameToExplanation["argument_list"] = "Argument List";
    m_ruleNameToExplanation["expression"] = "Expression";
}

struct AdditiveExprTag : Annotation
{
};
using AdditiveExprRule = x3::rule<AdditiveExprTag, ast::Expression>;
AdditiveExprRule const AdditiveExpr = "additive_expr";

struct MultiplicativeExprTag : Annotation
{
};
using MultiplicativeExprRule = x3::rule<MultiplicativeExprTag, ast::Expression>;
MultiplicativeExprRule const MultiplicativeExpr = "multiplicative_expr";

struct UnaryExprTag : Annotation
{
};
using UnaryExprRule = x3::rule<UnaryExprTag, ast::Operand>;
UnaryExprRule const UnaryExpr = "unary_expr";

struct ImaginaryTag;
using ImaginaryRule = x3::rule<ImaginaryTag, ast::Imaginary>;
ImaginaryRule const Imaginary = "imaginary";

struct PrimaryExprTag;
using PrimaryExprRule = x3::rule<PrimaryExprTag, ast::Operand>;
PrimaryExprRule const PrimaryExpr = "primary_expr";

struct ArgumentListTag;
using ArgumentListRule = x3::rule<ArgumentListTag, std::vector<ast::Expression>>;
ArgumentListRule const ArgumentList = "argument_list";

struct IdentifierTag;
using IdentifierRule = x3::rule<IdentifierTag, std::string>;
IdentifierRule const Identifier = "identifier";

struct FunctionCallTag : Annotation
{
};
using FunctionCallRule = x3::rule<FunctionCallTag, ast::FunctionCall>;
FunctionCallRule const FunctionCall = "function_call";

struct ExpressionTag : Annotation, OnFailure
{
};
using ExpressionRule = x3::rule<ExpressionTag, ast::Expression>;
ExpressionRule const Expression = "expression";

// clang-format off
auto const Imaginary_def =
    x3::lit('i') >> x3::attr(1.0)
    | x3::double_ >> x3::lit('i');

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
    | (x3::char_('+') > PrimaryExpr);

auto const ArgumentList_def = Expression % ',';

auto const Identifier_def =
    x3::raw[x3::lexeme[(x3::alpha | '_') >> *(x3::alnum | '_')]];

auto const FunctionCall_def =
    Identifier >> -('(' > ArgumentList > ')');

auto const PrimaryExpr_def =
    Imaginary
    | x3::double_
    | FunctionCall
    | '(' > Expression > ')';

auto const Expression_def = AdditiveExpr;
// clang-format on

BOOST_SPIRIT_DEFINE(Imaginary, AdditiveExpr, MultiplicativeExpr, UnaryExpr, ArgumentList, Identifier, FunctionCall,
                    PrimaryExpr, Expression);

using Iterator = std::string::const_iterator;
using PhraseContext = x3::phrase_parse_context<x3::ascii::space_type>::type;
using Context = x3::context<ErrorHandlerTag, std::reference_wrapper<ErrorHandler<Iterator>> const, PhraseContext>;

BOOST_SPIRIT_INSTANTIATE(ExpressionRule, Iterator, Context);

bool parseFormula(const std::string &text, ast::Expression &value, std::string &errors)
{
    using ascii::space;
    using x3::double_;
    using x3::phrase_parse;

    using Iterator = std::string::const_iterator;
    Iterator       begin = text.begin();
    const Iterator end = text.end();

    std::ostringstream     str;
    ErrorHandler<Iterator> errorHandler(begin, end, str);
    auto const             parser = x3::with<ErrorHandlerTag>(std::ref(errorHandler))[Expression];
    const bool             result = phrase_parse(begin, end, parser, space, value) && begin == end;
    errors = str.str();
    return result;
}

bool parseFormula(const std::string &text, ast::Expression &value)
{
    std::string errors;
    const bool  result = parseFormula(text, value, errors);
    if (!result)
    {
        std::cerr << errors << '\n';
    }
    return result;
}

} // namespace fractalFormula
