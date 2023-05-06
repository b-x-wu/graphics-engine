#ifndef HITTABLE_HEADER
#define HITTABLE_HEADER

#include <memory>
#include "math.h"
#include "util.h"

class Hittable
{
public:
    virtual bool hit(Math::Ray ray, float t0, float t1, std::shared_ptr<Util::HitRecord> & hitRecord) const = 0;
};

#endif
