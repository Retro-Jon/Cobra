#pragma once

#include "Core.hpp"

namespace Cobra
{
    class COBRA_API ViewPort
    {
        private:
            int pos_x, pos_y, width, height;
        
        public:
            ViewPort(int x, int y, int w, int h);
            ~ViewPort();
            void MakeRenderTarget();
    };
}
