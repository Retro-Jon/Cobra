#include "headers/UI.hpp"

namespace Cobra
{
    UI::UI()
    {

    }

    UI::~UI()
    {

    }

    void UI::SetTransformation(int n_x, int n_y, int n_w, int n_h)
    {
        transformation.x = n_x;
        transformation.y = n_y;
        transformation.w = n_w;
        transformation.h = n_h;
    }

    UIpos UI::GetTransformation()
    {
        return transformation;
    }
}
