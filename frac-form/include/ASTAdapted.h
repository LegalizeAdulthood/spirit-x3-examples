#ifndef FRAC_FORM_AST_ADAPTED_H
#define FRAC_FORM_AST_ADAPTED_H

#include <AST.h>

#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
    fractalFormula::ast::Signed,
    (char, sign)
    (fractalFormula::ast::Operand, operand)
)

BOOST_FUSION_ADAPT_STRUCT(
    fractalFormula::ast::Imaginary,
    (double, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    fractalFormula::ast::Operation,
    (char, operation)
    (fractalFormula::ast::Operand, operand)
)

BOOST_FUSION_ADAPT_STRUCT(
    fractalFormula::ast::Expression,
    (fractalFormula::ast::Operand, first)
    (std::vector<fractalFormula::ast::Operation>, rest)
)

BOOST_FUSION_ADAPT_STRUCT(
    fractalFormula::ast::FunctionCall,
    (std::string, name)
    (std::vector<fractalFormula::ast::Expression>, arguments)
)

#endif
