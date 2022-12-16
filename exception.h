#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <stdexcept>
#include <string>

namespace PH {
class Exception : public std::exception{
public:
    Exception(const std::string& msg, int code = 0);

    Exception(const std::string& msg, const std::string& arg, int code = 0);

    Exception(const std::string& msg, const Exception& nested, int code = 0);

    Exception(const Exception& exc);

    ~Exception()noexcept;

    Exception& operator =(const Exception& exc);

    virtual const char* name()const noexcept;

    virtual const char* className()const noexcept;

    virtual const char* what() const noexcept;

    const Exception* nested()const;

    const std::string& message()const;

    int code()const;

    std::string displayText()const;

    virtual Exception* clone()const;

    virtual void rethrow()const;

protected:
    explicit Exception(int code=0);

    void message(const std::string& msg);

    void extendedMessage(const std::string& arg);
private:
    std::string _msg;
    Exception* _pNested;
    int _code;
};


inline const Exception* Exception::nested()const {
    return _pNested;
}

inline const std::string& Exception::message()const {
    return _msg;
}

inline void Exception::message(const std::string& msg) {
    _msg = msg;
}

inline int Exception::code()const {
    return _code;
}

#define PH_DECLARE_EXCEPYION_CODE(CLS,BASE,CODE) \
    class CLS : public BASE {\
        public:\
            CLS(int code=CODE);\
            CLS(const std::string& msg,int code=CODE);\
            CLS(const std::string& msg,const std::string& allocator_arg,int code=CODE);\
            CLS(const std::string& msg, const Exception& exc, int code = CODE);\
            CLS(const CLS& exc);\
            ~CLS()noexcept;\
            CLS& operator=(const CLS& exc);\
            const char* name()const noexcept;\
            const char* className()const noexcept;\
            Exception* clone()const;\
            void rethrow()const;\
    };


#define PH_DECLARE_EXCEPTION(CLS,BASE)\
    PH_DECLARE_EXCEPYION_CODE(CLS,BASE,0)

#define PH_IMPLEMENT_EXCEPTION(CLASS,BASE,NAME)\
    CLASS::CLASS(int code):BASE(code){\
    }\
    CLASS::CLASS(const std::string& MSG,const std::string& arg,int code):BASE(msg ,arg,code){\
    }\
    CLASS::CLASS(const std::string& msg ,const Exception& exc,int code)::BASE(msg,exc ,code){\
    }\
    CLASS::CLASS(const CLASS& exc):BASE(exc){\
    }\
    CLASS::~CLASS()noexcept {\
    }\
    CLASS& CLASS::operator=(const CLASS &exc){\
        BASE::operator=(exc);\
        return *this;\
    }\
    const char* CLASS::name()const noexcept {\
        return NAME;\
    }\
    const char* CLASS::className()const noexcept{\
        return typeid(*this).name();\
    }\
    Exception* CLASS::clone()const{\
        return new CLASS(*this);\
    }\
    void CLASS::rethow()const{\
        throw *this;\
    }
    

}

#endif // EXCEPTION_H
