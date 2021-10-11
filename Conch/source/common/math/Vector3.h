#ifndef __Vector3__H__
#define __Vector3__H__

class Vector3
{
public:

    Vector3() : x(0.0f),y(0.0f),z(0.0f)
    {}

    Vector3(float x, float y, float z) : x(x), y(y),z(z)
    {}

    friend Vector3 operator*(Vector3 const & v, float scalar)
    {
        return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
    }

    friend Vector3 operator*(float scalar, Vector3 const & v)
    {
        return Vector3(scalar * v.x, scalar * v.y,scalar * v.z);
    }

    Vector3& operator*=(float scalar)
    {
        this->x *= scalar;
        this->y *= scalar;
        this->z *= scalar;
        return *this;
    }

    Vector3& operator+=(const Vector3& b)
    {
        x += b.x;
        y += b.y;
        z += b.z;
        return *this;
    }

    Vector3& operator-=(const Vector3& b)
    {
        x -= b.x;
        y -= b.y;
        z -= b.z;
        return *this;
    }

    Vector3 operator+(const Vector3& b) const
    {
        Vector3 ret = *this;
        ret += b;
        return ret;
    }

    Vector3 operator-(const Vector3& b) const
    {
        Vector3 ret = *this;
        ret -= b;
        return ret;
    }

    bool operator<=(Vector3 const & v)
    {
        return x <= v.x && y <= v.y && z <= v.z;
    }

    bool operator>=(Vector3 const & v)
    {
        return x >= v.x && y >= v.y && z >= v.z;
    }

    Vector3 cross(const Vector3& rkVector) const
    {
        return Vector3(
            y * rkVector.z - z * rkVector.y,
            z * rkVector.x - x * rkVector.z,
            x * rkVector.y - y * rkVector.x);
    }

    float dot(const Vector3& vec) const
    {
        return x * vec.x + y * vec.y + z * vec.z;
    }

public:

    float x;
    float y;
    float z;
};
#endif