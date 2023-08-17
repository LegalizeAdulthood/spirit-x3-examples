#ifndef FRAC_FORM_COMPLEX_H
#define FRAC_FORM_COMPLEX_H

#include <iostream>

namespace fractalFormula
{

struct Complex
{
    double re;
    double im;
};

inline bool operator==(const Complex &lhs, const Complex &rhs)
{
    return lhs.re == rhs.re && lhs.im == rhs.im;
}

inline bool operator!=(const Complex &lhs, const Complex &rhs)
{
    return !(lhs == rhs);
}

inline std::ostream &operator<<(std::ostream &str, const Complex &value)
{
    return str << '(' << value.re << ", " << value.im << ')';
}

} // namespace fractalFormula

#endif
