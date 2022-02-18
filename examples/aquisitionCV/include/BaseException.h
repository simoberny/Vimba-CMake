#ifndef BASE_EXCEPTION_H_
#define BASE_EXCEPTION_H_

#include <exception>
#include <string>

#include "VimbaCPP/Include/VimbaCPP.h"

namespace AVT {
namespace VmbAPI {

class BaseException: public std::exception
{
    std::string     m_Function;
    std::string     m_Message;
    VmbErrorType    m_Result;
public:
    BaseException( const char* const &fun, const char* const& msg, VmbErrorType result )
        : m_Result ( result )
    {
        try
        {
            if( NULL != fun)
            {
                m_Function = std::string( fun );
            }
        }
        catch(...) {}
        try
        {
            if( NULL != msg)
            {
                m_Message = std::string( msg );
            }
        }
        catch(...) {}
    }
    ~BaseException() throw()
    {}
    const std::string& Function() const
    {
        return m_Function;
    }
    const std::string& Message() const
    {
        return m_Message;
    }
    VmbErrorType Result() const
    {
        return m_Result;
    }
};

}}

#endif