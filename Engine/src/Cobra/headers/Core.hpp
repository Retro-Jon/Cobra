#pragma once

#ifndef APP
    #define COBRA_API __declspec(dllexport)
#else
    #define COBRA_API __declspec(dllimport)
#endif