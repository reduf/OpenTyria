#pragma once

float Vec2f_Dist2(Vec2f left, Vec2f right)
{
    float dx = right.x - left.x;
    float dy = right.y - left.y;
    return sqrtf(dx * dx + dy * dy);
}
