#include "Math/TeVector3.h"
#include "Math/TeVector4.h"
#include "Math/TeMath.h"

namespace te
{
    Vector3::Vector3(const Vector4& vec)
        :x(vec.x), y(vec.y), z(vec.z)
    {

    }

    Radian Vector3::AngleBetween(const Vector3& dest) const
    {
        float lenProduct = Length() * dest.Length();

        // Divide by zero check
        if (lenProduct < 1e-6f)
            lenProduct = 1e-6f;

        float f = Dot(dest) / lenProduct;

        f = Math::Clamp(f, -1.0f, 1.0f);
        return Math::Acos(f);
    }

    Vector3 Vector3::Normalize(const Vector3& val)
    {
        float len = Math::Sqrt(val.x * val.x + val.y * val.y + val.z * val.z);

        // Will also work for zero-sized vectors, but will change nothing
        if (len > 1e-08)
        {
            float invLen = 1.0f / len;

            Vector3 normalizedVec;
            normalizedVec.x = val.x * invLen;
            normalizedVec.y = val.y * invLen;
            normalizedVec.z = val.z * invLen;

            return normalizedVec;
        }
        else
            return val;
    }

    bool Vector3::IsNaN() const
    {
        return Math::IsNaN(x) || Math::IsNaN(y) || Math::IsNaN(z);
    }

    const Vector3 Vector3::ZERO{ TE_ZERO() };
    const Vector3 Vector3::ONE(1, 1, 1);
    const Vector3 Vector3::INF =
        Vector3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());

    const Vector3 Vector3::UNIT_X(1, 0, 0);
    const Vector3 Vector3::UNIT_Y(0, 1, 0);
    const Vector3 Vector3::UNIT_Z(0, 0, 1);
}
