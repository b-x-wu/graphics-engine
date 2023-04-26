#ifndef MATH_HEADER
#define MATH_HEADER

namespace Math
{
    class Vector3
    {
    public:
        Vector3(float x, float y, float z);
        float getX() const;
        float getY() const;
        float getZ() const;

        Vector3& operator+=(Vector3 const& rhs);
        Vector3& operator-=(Vector3 const& rhs);

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

    float dot(Vector3 const& lhs, Vector3 const& rhs);
    Vector3 cross(Vector3 const& lhs, Vector3 const& rhs);
}

#endif
