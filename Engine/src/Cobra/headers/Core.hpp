#pragma once

#ifndef APP
    #define COBRA_API __declspec(dllexport)
#else
    #define COBRA_API __declspec(dllimport)
#endif

#ifndef GL_INCLUDES
    #define GLEW_STATIC
    #include <GL/glew.h>
    #define GLFW_INCLUDE_NONE
    #include <GLFW/glfw3.h>
    #define GL_INCLUDES
#endif