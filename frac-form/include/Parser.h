#ifndef FRAC_FORM_PARSER_H
#define FRAC_FORM_PARSER_H

#include <string>

namespace fractalFormula
{

struct Complex
{
    double re;
    double im;
};

bool parse(const std::string &text, Complex &value);

} // namespace fractalFormula

#endif
