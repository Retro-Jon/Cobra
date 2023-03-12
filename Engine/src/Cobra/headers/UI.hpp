#pragma once

#include "Core.hpp"

namespace Cobra
{
    struct UIpos
    {
        int x, y, w, h;
    };

    class UI
    {
        private:
            UIpos transformation;
        
        public:
            UI();
            ~UI();

            void SetTransformation(int n_x, int n_y, int n_w, int n_h);
            UIpos GetTransformation();
    };
}