#include "parser.h"

#include <boost/spirit/home/x3.hpp>

namespace x3 = boost::spirit::x3;
namespace ascii = x3::ascii;

namespace fractalFormula
{

bool parse(const std::string &text, double &value)
{
    using x3::double_;
    using x3::phrase_parse;
    using ascii::space;

    std::string::const_iterator       begin = text.begin();
    const std::string::const_iterator end = text.end();
    const bool                        result = phrase_parse(begin, end, double_, space, value);
    return result && begin == end;
}

} // namespace fractalFormula
