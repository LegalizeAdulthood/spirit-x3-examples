#ifndef FRAC_FORM_PARSER_H
#define FRAC_FORM_PARSER_H

#include "Complex.h"

#include <string>

namespace fractalFormula
{

bool parse(const std::string &text, Complex &value);

} // namespace fractalFormula

#endif
