#include "headers/ViewPort.hpp"

namespace Cobra
{
    ViewPort::ViewPort(int x, int y, int w, int h)
    {
        UI::SetTransformation(x, y, w, h);

        glewInit();
    }

    ViewPort::~ViewPort()
    {
    }

    void ViewPort::MakeRenderTarget()
    {
        glBindRenderbuffer(GL_DRAW_BUFFER, 0);
    }

    void ViewPort::BindCamera(const char* camera_name)
    {
        camera = camera_name;
    }

    const char* ViewPort::GetBoundCamera()
    {
        return camera;
    }
}