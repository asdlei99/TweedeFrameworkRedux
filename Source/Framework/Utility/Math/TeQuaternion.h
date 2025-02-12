#pragma once

#include "Prerequisites/TePrerequisitesUtility.h"
#include "Math/TeMath.h"
#include "Math/TeVector3.h"

namespace te
{
     /** Represents a quaternion used for 3D rotations. */
    class TE_UTILITY_EXPORT Quaternion
    {
    private:
        struct EulerAngleOrderData
        {
            int a, b, c;
        };

    public:
        Quaternion() = default;
        constexpr Quaternion(const Quaternion&) = default;
        constexpr Quaternion& operator=(const Quaternion&) = default;

        constexpr Quaternion(TE_ZERO zero)
            : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
        { }

        constexpr Quaternion(TE_IDENTITY)
            : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
        { }

        constexpr Quaternion(float w, float x, float y, float z)
            : x(x), y(y), z(z), w(w)
        { }

        /** Construct a quaternion from a rotation matrix. */
        explicit Quaternion(const Matrix3& rot)
        {
            FromRotationMatrix(rot);
        }

        /** Construct a quaternion from an angle/axis. */
        explicit Quaternion(const Vector3& axis, const Radian& angle)
        {
            FromAxisAngle(axis, angle);
        }

        /** Construct a quaternion from 3 orthonormal local axes. */
        explicit Quaternion(const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis)
        {
            FromAxes(xaxis, yaxis, zaxis);
        }

        /**
         * Construct a quaternion from euler angles, YXZ ordering.
         *
         * @see		Quaternion::FromEulerAngles
         */
        explicit Quaternion(const Radian& xAngle, const Radian& yAngle, const Radian& zAngle)
        {
            FromEulerAngles(xAngle, yAngle, zAngle);
        }

        /**
         * Construct a quaternion from euler angles, custom ordering.
         *
         * @see		Quaternion::FromEulerAngles
         */
        explicit Quaternion(const Radian& xAngle, const Radian& yAngle, const Radian& zAngle, EulerAngleOrder order)
        {
            FromEulerAngles(xAngle, yAngle, zAngle, order);
        }

        /** Exchange the contents of this quaternion with another. */
        void Swap(Quaternion& other)
        {
            std::swap(w, other.w);
            std::swap(x, other.x);
            std::swap(y, other.y);
            std::swap(z, other.z);
        }

        float operator[] (const size_t i) const
        {
            assert(i < 4);

            return *(&x + i);
        }

        float& operator[] (const size_t i)
        {
            assert(i < 4);

            return *(&x + i);
        }

        /**
         * Initializes the quaternion from a 3x3 rotation matrix.
         *
         * @note	It's up to the caller to ensure the matrix is orthonormal.
         */
        void FromRotationMatrix(const Matrix3& mat);

        /**
         * Initializes the quaternion from an angle axis pair. Quaternion will represent a rotation of "angle" radians
         * around "axis".
         */
        void FromAxisAngle(const Vector3& axis, const Radian& angle);

        /**
         * Initializes the quaternion from orthonormal set of axes. Quaternion will represent a rotation from base axes
         * to the specified set of axes.
         *
         * @note	It's up to the caller to ensure the axes are orthonormal.
         */
        void FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);

        /**
         * Creates a quaternion from the provided Pitch/Yaw/Roll angles.
         *
         * @param[in]	xAngle	Rotation about x axis. (AKA Pitch)
         * @param[in]	yAngle	Rotation about y axis. (AKA Yaw)
         * @param[in]	zAngle	Rotation about z axis. (AKA Roll)
         *
         * @note
         * Since different values will be produced depending in which order are the rotations applied, this method assumes
         * they are applied in YXZ order. If you need a specific order, use the overloaded fromEulerAngles() method instead.
         */
        void FromEulerAngles(const Radian& xAngle, const Radian& yAngle, const Radian& zAngle);

        /**
         * Creates a quaternion from the provided Pitch/Yaw/Roll angles.
         *
         * @param[in]	xAngle	Rotation about x axis. (AKA Pitch)
         * @param[in]	yAngle	Rotation about y axis. (AKA Yaw)
         * @param[in]	zAngle	Rotation about z axis. (AKA Roll)
         * @param[in]	order 	The order in which rotations will be extracted. Different values can be retrieved depending
         *						on the order.
         */
        void FromEulerAngles(const Radian& xAngle, const Radian& yAngle, const Radian& zAngle, EulerAngleOrder order);

        /**
         * Converts a quaternion to a rotation matrix.
         */
        void ToRotationMatrix(Matrix3& mat) const;

        /**
         * Converts a quaternion to an angle axis pair.
         *
         * @param[out]	axis 	The axis around the which rotation takes place.
         * @param[out]	angle	The angle in radians determining amount of rotation around the axis.
         */
        void ToAxisAngle(Vector3& axis, Radian& angle) const;

        /**
         * Converts a quaternion to an orthonormal set of axes.
         *
         * @param[out]	xAxis	The X axis.
         * @param[out]	yAxis	The Y axis.
         * @param[out]	zAxis	The Z axis.
         */
        void ToAxes(Vector3& xAxis, Vector3& yAxis, Vector3& zAxis) const;

        /**
         * Extracts Pitch/Yaw/Roll rotations from this quaternion.
         *
         * @param[out]	xAngle	Rotation about x axis. (AKA Pitch)
         * @param[out]	yAngle  Rotation about y axis. (AKA Yaw)
         * @param[out]	zAngle 	Rotation about z axis. (AKA Roll)
         *
         * @return	True if unique solution was found, false otherwise.
         */
        bool ToEulerAngles(Radian& xAngle, Radian& yAngle, Radian& zAngle) const;

        /** Gets the positive x-axis of the coordinate system transformed by this quaternion. */
        Vector3 XAxis() const;

        /** Gets the positive y-axis of the coordinate system transformed by this quaternion. */
        Vector3 YAxis() const;

        /** Gets the positive z-axis of the coordinate system transformed by this quaternion. */
        Vector3 ZAxis() const;


        Quaternion operator+ (const Quaternion& rhs) const
        {
            return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
        }

        Quaternion operator- (const Quaternion& rhs) const
        {
            return Quaternion(w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z);
        }

        Quaternion operator* (const Quaternion& rhs) const
        {
            return Quaternion
            (
                w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
                w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
                w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
                w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x
            );
        }

        Quaternion operator* (float rhs) const
        {
            return Quaternion(rhs * w, rhs * x, rhs * y, rhs * z);
        }

        Vector3 operator*(const Vector3& rhs) const
        {
            const Vector3 qVec(x, y, z);
            const Vector3 cross1(qVec.Cross(rhs));
            const Vector3 cross2(qVec.Cross(cross1));

            return rhs + 2.0f * (cross1 * w + cross2);
        }

        Quaternion operator/ (float rhs) const
        {
            assert(rhs != 0.0);

            const float inv = 1.0f / rhs;
            return Quaternion(w * inv, x * inv, y * inv, z * inv);
        }

        Quaternion operator- () const
        {
            return Quaternion(-w, -x, -y, -z);
        }

        bool operator== (const Quaternion& rhs) const
        {
            return (rhs.x == x) && (rhs.y == y) && (rhs.z == z) && (rhs.w == w);
        }

        bool operator!= (const Quaternion& rhs) const
        {
            return !operator==(rhs);
        }

        Quaternion& operator+= (const Quaternion& rhs)
        {
            w += rhs.w;
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;

            return *this;
        }

        Quaternion& operator-= (const Quaternion& rhs)
        {
            w -= rhs.w;
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;

            return *this;
        }

        Quaternion& operator*= (const Quaternion& rhs)
        {
            float newW = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
            float newX = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
            float newY = w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z;
            float newZ = w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x;

            w = newW;
            x = newX;
            y = newY;
            z = newZ;

            return *this;
        }

        friend Quaternion operator* (float lhs, const Quaternion& rhs)
        {
            return Quaternion(lhs * rhs.w, lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
        }

        /** Calculates the dot product of this quaternion and another. */
        float Dot(const Quaternion& other) const
        {
            return w * other.w + x * other.x + y * other.y + z * other.z;
        }

        /** Normalizes this quaternion, and returns the previous length. */
        template<bool SAFE = true>
        float Normalize(float tolerance = 1e-04f)
        {
            float len = Math::Sqrt(Dot(*this, *this));
            if (!SAFE || len > (tolerance * tolerance))
                *this = *this * (1.0f / len);

            return len;
        }

        /**
         * Gets the inverse.
         *
         * @note	Quaternion must be non-zero.
         */
        Quaternion Inverse() const;

        /** Rotates the provided vector. */
        Vector3 Rotate(const Vector3& vec) const;

        /**
         * Orients the quaternion so its negative z axis points to the provided direction.
         *
         * @param[in]	forwardDir	Direction to orient towards.
         */
        void LookRotation(const Vector3& forwardDir);

        /**
         * Orients the quaternion so its negative z axis points to the provided direction.
         *
         * @param[in]	forwardDir	Direction to orient towards.
         * @param[in]	upDir		Constrains y axis orientation to a plane this vector lies on. This rule might be broken
         *							if forward and up direction are nearly parallel.
         */
        void LookRotation(const Vector3& forwardDir, const Vector3& upDir);

        /** Query if any of the components of the quaternion are not a number. */
        bool IsNaN() const
        {
            return Math::IsNaN(x) || Math::IsNaN(y) || Math::IsNaN(z) || Math::IsNaN(w);
        }

        /** Calculates the dot product between two quaternions. */
        static float Dot(const Quaternion& lhs, const Quaternion& rhs)
        {
            return lhs.w * rhs.w + lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
        }

        /** Normalizes the provided quaternion. */
        template<bool SAFE = true>
        static Quaternion Normalize(const Quaternion& q, const float& tolerance = 1e-04f)
        {
            float sqrdLen = Dot(q, q);
            if (!SAFE || sqrdLen > tolerance)
                return q * Math::InvSqrt(sqrdLen);

            return q;
        }

        /**
         * Performs spherical interpolation between two quaternions. Spherical interpolation neatly interpolates between
         * two rotations without modifying the size of the vector it is applied to (unlike linear interpolation).
         */
        static Quaternion Slerp(const float& t, const Quaternion& p, const Quaternion& q, bool shortestPath = true);

        /**
         * Linearly interpolates between the two quaternions using @p t. t should be in [0, 1] range, where t = 0
         * corresponds to the left vector, while t = 1 corresponds to the right vector.
         */
        static Quaternion Lerp(const float& t, const Quaternion& a, const Quaternion& b)
        {
            float d = Dot(a, b);
            float flip = d >= 0.0f ? 1.0f : -1.0f;

            Quaternion output = flip * (1.0f - t) * a + t * b;
            return Normalize(output);
        }

        /** Gets the shortest arc quaternion to rotate this vector to the destination vector. */
        static Quaternion GetRotationFromTo(const Vector3& from, const Vector3& dest, const Vector3& fallbackAxis = Vector3::ZERO);

        /** Returns the minimum of all the quaternion components as a new quaternion. */
        static Quaternion Min(const Quaternion& a, const Quaternion& b)
        {
            return Quaternion(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w));
        }

        /** Returns the maximum of all the quaternion components as a new quaternion. */
        static Quaternion Max(const Quaternion& a, const Quaternion& b)
        {
            return Quaternion(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w));
        }

        static constexpr const float EPSILON = 1e-03f;

        static const Quaternion ZERO;
        static const Quaternion IDENTITY;

        float x, y, z, w; // Note: Order is relevant, don't break it
    };
}

/** @cond SPECIALIZATIONS */
namespace std
{
    template<> class numeric_limits<te::Quaternion>
    {
    public:
        constexpr static te::Quaternion infinity()
        {
            return te::Quaternion(
                std::numeric_limits<float>::infinity(),
                std::numeric_limits<float>::infinity(),
                std::numeric_limits<float>::infinity(),
                std::numeric_limits<float>::infinity());
        }
    };
}
/** @endcond */
