#pragma once

#include "Core.hpp"
#include "UI.hpp"
#include <string>

namespace Cobra
{
    class COBRA_API ViewPort : public UI
    {
        private:
            std::string camera;
        
        public:
            ViewPort(int x, int y, int w, int h);
            ~ViewPort();
            void BindCamera(std::string camera_name);
            
            std::string GetBoundCamera();
    };
}
