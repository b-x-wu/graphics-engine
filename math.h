#ifndef MATH_HEADER
#define MATH_HEADER

#include <ostream>

namespace Math
{
    class Vector3
    {
    public:
        Vector3();
        Vector3(Vector3 const& v);
        Vector3(float x, float y, float z);
        float getX() const;
        float getY() const;
        float getZ() const;

        Vector3& operator+=(Vector3 const& rhs);
        Vector3& operator-=(Vector3 const& rhs);
        Vector3 operator-() const;

        float norm() const;

    private:
        float x, y, z;
        void setX(float f);
        void setY(float f);
        void setZ(float f);
    };

    Vector3 operator+(Vector3 const& lhs, Vector3 const& rhs);
    Vector3 operator-(Vector3 const& lhs, Vector3 const& rhs);
    bool operator==(Vector3 const& lhs, Vector3 const& rhs);
    bool operator!=(Vector3 const& lhs, Vector3 const& rhs);
    Vector3 operator*(float const& lhs, Vector3 const& rhs);
    Vector3 operator*(Vector3 const& lhs, float const& rhs);
    Vector3 operator/(Vector3 const& lhs, float const& rhs);
    std::ostream& operator<<(std::ostream &strm, const Vector3 &v);

    float dot(Vector3 const& lhs, Vector3 const& rhs);
    Vector3 cross(Vector3 const& lhs, Vector3 const& rhs);

    struct Ray
    {
        Vector3 origin;
        Vector3 direction;
    };

    // axis aligned box
    class Box
    {
    public:
        Vector3 min, max; // the vectors containing all min values and all max values respectively

        Box(); // create the degenerate box that has min and max set to the origin
        Box(Vector3 u, Vector3 v);

        bool isInside(Vector3 p) const; // not edge inclusive
    };
}

#endif
