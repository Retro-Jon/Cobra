#include "headers/ViewPort.hpp"

#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Cobra
{
    ViewPort::ViewPort(int x, int y, int w, int h)
    {
        pos_x = x;
        pos_y = y;
        width = w;
        height = h;

        glewInit();
    }

    ViewPort::~ViewPort()
    {
    }

    void ViewPort::MakeRenderTarget()
    {
        glBindRenderbuffer(GL_DRAW_BUFFER, 0);
        glViewport(pos_x, pos_y, width, height);
    }
}