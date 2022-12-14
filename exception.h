#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <stdexcept>
#include <string>

namespace PH {
class Exception : public std::exception{
public:
    explicit Exception(const std::string& str);

    ~Exception()noexcept;

    virtual const char* what() const noexcept;

private:
    std::string m_str;
};

}

#endif // EXCEPTION_H
