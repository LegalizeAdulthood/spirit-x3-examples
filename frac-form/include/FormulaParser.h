#ifndef FRAC_FORM_FORMULA_PARSER_H
#define FRAC_FORM_FORMULA_PARSER_H

#include <AST.h>

#include <string>

namespace fractalFormula
{

bool parseFormula(const std::string &text, ast::Expression &value, std::string &errors);
bool parseFormula(const std::string &text, ast::Expression &value);

} // namespace fractalFormula

#endif
