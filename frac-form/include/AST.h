#ifndef FRAC_FORM_AST_H
#define FRAC_FORM_AST_H

#include <Complex.h>

#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/fusion/include/io.hpp>

#include <vector>

namespace fractalFormula
{

namespace ast
{

namespace x3 = boost::spirit::x3;

struct Nil {};
struct Signed;
struct Imaginary;
struct Expression;
struct FunctionCall;

struct Operand : x3::variant<Nil, double, x3::forward_ast<Imaginary>, x3::forward_ast<Signed>,
                             x3::forward_ast<Expression>, x3::forward_ast<FunctionCall>>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct Signed
{
    char    sign;
    Operand operand;
};

struct Imaginary
{
    double value;
};

struct Operation : x3::position_tagged
{
    char    operation; // operator is a reserved word
    Operand operand;
};

struct Expression : x3::position_tagged
{
    Operand                first;
    std::vector<Operation> rest;
};

struct FunctionCall : x3::position_tagged
{
    std::string             name;
    std::vector<Expression> arguments;
};

inline std::ostream &operator<<(std::ostream &str, Nil)
{
    return str << "nil";
}

} // namespace ast

} // namespace fractalFormula

#endif
