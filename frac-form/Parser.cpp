#include "parser.h"

#include <boost/fusion/adapted.hpp>
#include <boost/spirit/home/x3.hpp>

namespace x3 = boost::spirit::x3;
namespace ascii = x3::ascii;

namespace fractalFormula
{

auto assign = [](auto &ctx)
{
    _val(ctx) = Complex{_attr(ctx), 0.0};
};

struct Constant;
x3::rule<Constant, Complex> const constant = "constant";
auto const                        constant_def = x3::double_[assign];
BOOST_SPIRIT_DEFINE(constant);

bool parse(const std::string &text, Complex &value)
{
    using ascii::space;
    using x3::double_;
    using x3::phrase_parse;

    using Iterator = std::string::const_iterator;
    Iterator       begin = text.begin();
    const Iterator end = text.end();
    const bool     result = phrase_parse(begin, end, constant, space, value) && begin == end;
    return result;
}

} // namespace fractalFormula
