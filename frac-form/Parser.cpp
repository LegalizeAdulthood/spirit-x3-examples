#include "parser.h"

#include <boost/fusion/adapted.hpp>
#include <boost/spirit/home/x3.hpp>

namespace x3 = boost::spirit::x3;
namespace ascii = x3::ascii;

namespace fractalFormula
{

auto assignReal = [](auto &ctx) { _val(ctx) = Complex{_attr(ctx), 0.0}; };

struct Real;
x3::rule<Real, Complex> const real = "real";
auto const                    real_def = x3::double_[assignReal];
BOOST_SPIRIT_DEFINE(real);

auto assignI = [](auto &ctx) { _val(ctx) = Complex{0.0, 1.0}; };
auto assignImaginary = [](auto &ctx) { _val(ctx) = Complex{0.0, _attr(ctx)}; };

struct Imaginary;
x3::rule<Imaginary, Complex> const imaginary = "imaginary";
auto const imaginary_def = x3::lit('i')[assignI] | (x3::double_ >> x3::lit('i'))[assignImaginary];
BOOST_SPIRIT_DEFINE(imaginary);

struct Literal;
x3::rule<Literal, Complex> const literal = "literal";
auto const                       literal_def = imaginary | real;
BOOST_SPIRIT_DEFINE(literal);

bool parse(const std::string &text, Complex &value)
{
    using ascii::space;
    using x3::double_;
    using x3::phrase_parse;

    using Iterator = std::string::const_iterator;
    Iterator       begin = text.begin();
    const Iterator end = text.end();
    const bool     result = phrase_parse(begin, end, literal, space, value) && begin == end;
    return result;
}

} // namespace fractalFormula
