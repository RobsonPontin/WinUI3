#pragma once

#include <cstdarg>
#include <stdio.h>
#include <debugapi.h>

namespace Multiprocess
{
    namespace Utils
    {
        struct Logger
        {
            static void Debug(const char* szFormat, ...)
            {
                char szBuff[1024];
                va_list arg;
                va_start(arg, szFormat);
                _vsnprintf_s(szBuff, sizeof(szBuff), _TRUNCATE, szFormat, arg);
                va_end(arg);

                OutputDebugStringA(szBuff);
            }
        };
    }
}
