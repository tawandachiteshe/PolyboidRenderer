#pragma once
#include "KeyCodes.h"
#include "glm/vec2.hpp"


namespace Polyboid
{
    class Input
    {
    public:
        static glm::vec2 GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
        

        static void SetCursorMode(CursorMode mode);
        
        static bool MousePressed(MouseCodes codes);
        static  bool MouseReleased(MouseCodes codes);
        static  bool MouseHeld(MouseCodes codes);

        static bool KeyPressed(KeyCodes codes);
        static  bool KeyReleased(KeyCodes codes);
        static  bool KeyHeld(KeyCodes codes);

        
        
    
    };

}

