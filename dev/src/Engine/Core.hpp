#pragma once

#define PLATFORM_WINDOWS
#define BUILD_DLL

#ifdef PLATFORM_WINDOWS
    #ifdef BUILD_DLL
        #define API __declspec(dllexport)
    #else
        #define API __declspec(dllimport)
    #endif
#else
    #error Only Supports Windows
#endif