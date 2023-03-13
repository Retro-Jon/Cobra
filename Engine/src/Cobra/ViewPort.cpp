#include "headers/ViewPort.hpp"

namespace Cobra
{
    ViewPort::ViewPort(int x, int y, int w, int h)
    {
        UI::SetTransformation(x, y, w, h);
    }

    ViewPort::~ViewPort()
    {
    }

    void ViewPort::BindCamera(std::string camera_name)
    {
        camera = camera_name;
    }

    std::string ViewPort::GetBoundCamera()
    {
        return camera;
    }
}