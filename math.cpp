#include "math.h"
#include <cmath>

Math::Vector3::Vector3()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
};

Math::Vector3::Vector3(Vector3 const& v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}

Math::Vector3::Vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
};

float Math::Vector3::getX() const
{
    return this->x;
};

float Math::Vector3::getY() const
{
    return this->y;
};

float Math::Vector3::getZ() const
{
    return this->z;
};

void Math::Vector3::setX(float f)
{
    this->x = f;
};

void Math::Vector3::setY(float f)
{
    this->y = f;
};

void Math::Vector3::setZ(float f)
{
    this->z = f;
};

Math::Vector3& Math::Vector3::operator+=(Math::Vector3 const& rhs)
{
    this->setX(this->getX() + rhs.getX());
    this->setY(this->getY() + rhs.getY());
    this->setZ(this->getZ() + rhs.getZ());

    return *this;
};

Math::Vector3& Math::Vector3::operator-=(Math::Vector3 const& rhs)
{
    this->setX(this->getX() - rhs.getX());
    this->setY(this->getY() - rhs.getY());
    this->setZ(this->getZ() - rhs.getZ());

    return *this;
};

Math::Vector3 Math::Vector3::operator-() const
{
    return { -this->getX(), -this->getY(), -this->getZ() };
};

float Math::Vector3::norm() const
{
    return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
};

Math::Vector3 Math::operator+(Math::Vector3 const& lhs, Math::Vector3 const& rhs)
{
    return { lhs.getX() + rhs.getX(), lhs.getY() + rhs.getY(), lhs.getZ() + rhs.getZ() };
};

Math::Vector3 Math::operator-(Math::Vector3 const& lhs, Math::Vector3 const& rhs)
{
    return { lhs.getX() - rhs.getX(), lhs.getY() - rhs.getY(), lhs.getZ() - rhs.getZ() };
};

bool Math::operator==(Math::Vector3 const& lhs, Math::Vector3 const& rhs)
{
    return (lhs.getX() == rhs.getX() && lhs.getY() == rhs.getY() && lhs.getZ() == rhs.getZ());
};

bool Math::operator!=(Math::Vector3 const& lhs, Math::Vector3 const& rhs)
{
    return (lhs.getX() != rhs.getX() || lhs.getY() != rhs.getY() || lhs.getZ() != rhs.getZ());
};
    
Math::Vector3 Math::operator*(float const& lhs, Math::Vector3 const& rhs)
{
    return { lhs * rhs.getX(), lhs * rhs.getY(), lhs * rhs.getZ() };
};

Math::Vector3 Math::operator*(Math::Vector3 const& lhs, float const& rhs)
{
    return { lhs.getX() * rhs, lhs.getY() * rhs, lhs.getZ() * rhs };    
};

Math::Vector3 Math::operator/(Math::Vector3 const& lhs, float const& rhs)
{
    return { lhs.getX() / rhs, lhs.getY() / rhs, lhs.getZ() / rhs };
};

float Math::dot(Math::Vector3 const& lhs, Math::Vector3 const& rhs)
{
    return lhs.getX() * rhs.getX() + lhs.getY() * rhs.getY() + lhs.getZ() * rhs.getZ();
}

Math::Vector3 Math::cross(Math::Vector3 const& lhs, Math::Vector3 const& rhs)
{
    float x = lhs.getY() * rhs.getZ() - lhs.getZ() * rhs.getY();
    float y = -(lhs.getX() * rhs.getZ() - lhs.getZ() * rhs.getX());
    float z = lhs.getX() * rhs.getY() - lhs.getY() * rhs.getX();

    return { x, y, z };
}

Math::Box::Box()
{
    this->min = { 0, 0, 0 };
    this->max = { 0, 0, 0 };
}

Math::Box::Box(Math::Vector3 u, Math::Vector3 v)
{
    this->min = { std::min(u.getX(), v.getX()), std::min(u.getY(), v.getY()), std::min(u.getZ(), v.getZ()) };
    this->max = { std::max(u.getX(), v.getX()), std::max(u.getY(), v.getY()), std::max(u.getZ(), v.getZ()) };
}

bool Math::Box::isInside(Math::Vector3 p) const
{
    if (p.getX() <= this->min.getX() || p.getX() >= this->max.getX()) { return false; };
    if (p.getY() <= this->min.getY() || p.getY() >= this->max.getY()) { return false; };
    return p.getZ() > this->min.getZ() && p.getZ() < this->max.getZ();
}
