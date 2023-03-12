#include "headers/ViewPort.hpp"

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