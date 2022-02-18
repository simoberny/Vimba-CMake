#ifndef STREAM_SYSTEM_INFO_H
#define STREAM_SYSTEM_INFO_H

#include <exception>

#include "VimbaCPP/Include/VimbaCPP.h"

namespace AVT {
namespace VmbAPI {

template<typename STREAM>
STREAM& operator<<( STREAM& os, AVT::VmbAPI::VimbaSystem &sys )
{
    VmbVersionInfo_t info;
    if (VmbErrorSuccess != sys.QueryVersion( info ))
    {
        throw std::exception();
    }
    os << info.major << "." << info.minor << "." << info.patch;
    return os;
}

}} // Namespace AVT::VmbAPI
#endif