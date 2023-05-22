#ifndef UTIL_HEADER
#define UTIL_HEADER

#include <stdint.h>
#include "math.h"

namespace Util
{
    struct Color
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };
    struct HitRecord
    {
        bool isValid = false;
        float intersectionTime = -1;
        Math::Vector3 unitNormal;
        Math::Vector3 intersectionPoint;
        int hitObjectIndex = -1;
    };
};

#endif
