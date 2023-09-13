#ifndef FRAC_FORM_AST_PRINTER_H
#define FRAC_FORM_AST_PRINTER_H

#include <AST.h>

#include <boost/variant/apply_visitor.hpp>

#include <ostream>
#include <stdexcept>

namespace fractalFormula
{
namespace ast
{

class Printer
{
public:
    using result_type = void;

    Printer(std::ostream &str) :
        m_str(str)
    {
    }

    void operator()(Nil) const;
    void operator()(double ast) const;
    void operator()(Imaginary ast) const;
    void operator()(const Signed &ast) const;
    void operator()(const Expression &ast) const;
    void operator()(const FunctionCall &ast) const;
    void operator()(const Operation &ast) const;

private:
    std::ostream &m_str;
};

inline void Printer::operator()(Nil) const
{
    throw std::runtime_error("Bad AST!");
}

inline void Printer::operator()(double ast) const
{
    m_str << ast;
}

inline void Printer::operator()(Imaginary ast) const
{
    m_str << ast.value << 'i';
}

inline void Printer::operator()(const Signed &ast) const
{
    if (ast.sign == '-')
    {
        m_str << ast.sign;
    }
    boost::apply_visitor(*this, ast.operand);
}

inline void Printer::operator()(const Expression &ast) const
{
    if (!ast.rest.empty())
    {
        m_str << '(';
    }

    boost::apply_visitor(*this, ast.first);
    for (const auto &rest : ast.rest)
    {
        (*this)(rest);
    }

    if (!ast.rest.empty())
    {
        m_str << ')';
    }
}

inline void Printer::operator()(const FunctionCall &ast) const
{
    m_str << ast.name;
    if (!ast.arguments.empty())
    {
        m_str << '(';
        bool first = true;
        for (const auto &arg : ast.arguments)
        {
            if (!first)
            {
                m_str << ", ";
            }
            first = false;
            (*this)(arg);
        }
        m_str << ')';
    }
}

inline void Printer::operator()(const Operation &ast) const
{
    m_str << ' ' << ast.operation << ' ';
    boost::apply_visitor(*this, ast.operand);
}

} // namespace ast
} // namespace fractalFormula

#endif
