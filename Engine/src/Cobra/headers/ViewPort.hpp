#pragma once

#include "Core.hpp"
#include "UI.hpp"

namespace Cobra
{
    class COBRA_API ViewPort : public UI
    {
        private:
            const char* camera;
        
        public:
            ViewPort(int x, int y, int w, int h);
            ~ViewPort();
            void MakeRenderTarget();
            void BindCamera(const char* camera_name);
            
            const char* GetBoundCamera();
    };
}
