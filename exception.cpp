#include "exception.h"

namespace PH {

Exception::Exception(const std::string &str):m_str(str)
{

}

Exception::~Exception() noexcept
{

}

const char *Exception::what() const noexcept
{
    return m_str.c_str();
}

}
