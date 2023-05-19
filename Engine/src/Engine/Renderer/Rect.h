#pragma once

namespace Polyboid
{
    struct Rect
    {
        Rect(float x = 0.0f, float y = 0.0f, float width = 1.0f, float height = 1.0f)
            : X(x)
              , Y(y)
              , Width(width)
              , Height(height)
        {
        }

        float X;
        float Y;
        float Width;
        float Height;
    };
}
