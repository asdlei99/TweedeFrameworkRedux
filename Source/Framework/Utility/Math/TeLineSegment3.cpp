#include "Math/TeLineSegment3.h"
#include "Math/TeRay.h"

namespace te
{
    LineSegment3::LineSegment3(const Vector3& start, const Vector3& end)
        : start(start), end(end)
    {

    }

    std::pair<std::array<Vector3, 2>, float>  LineSegment3::GetNearestPoint(const Ray& ray) const
    {
        const Vector3& org = ray.GetOrigin();
        const Vector3& dir = ray.GetDirection();

        Vector3 segDir = end - start;
        float segExtent = segDir.Normalize() * 0.5f;
        Vector3 segCenter = start + segDir * segExtent;

        Vector3 diff = org - segCenter;
        float a01 = -dir.Dot(segDir);
        float b0 = diff.Dot(dir);
        float c = diff.Dot(diff);
        float det = fabs(1.0f - a01 * a01);

        float s0, s1;
        float sqrDistance;
        if (det > 0.0f) // Not parallel
        {

            float b1 = -diff.Dot(segDir);
            s1 = a01 * b0 - b1;
            float extDet = segExtent * det;

            if (s1 >= -extDet)
            {
                if (s1 <= extDet) // Interior of the segment and interior of the ray are closest
                {
                    float invDet = 1.0f / det;
                    s0 = (a01*b1 - b0)*invDet;
                    s1 *= invDet;

                    sqrDistance = s0 * (s0 + a01 * s1 + 2.0f*b0) +
                        s1 * (a01*s0 + s1 + 2.0f*b1) + c;
                }
                else // Segment end and interior of the ray are closest
                {
                    s1 = segExtent;
                    s0 = -(a01*s1 + b0);
                    sqrDistance = -s0 * s0 + s1 * (s1 + (2.0f)*b1) + c;
                }
            }
            else // Segment start and interior of the ray are closest
            {
                s1 = -segExtent;
                s0 = -(a01*s1 + b0);
                sqrDistance = -s0 * s0 + s1 * (s1 + (2.0f)*b1) + c;
            }
        }
        else // Parallel
        {
            s1 = 0;
            s0 = -b0;
            sqrDistance = b0 * s0 + c;
        }

        if (sqrDistance < 0.0f)
            sqrDistance = 0.0f;

        float distance = std::sqrt(sqrDistance);

        std::array<Vector3, 2> nearestPoints;
        nearestPoints[0] = org + s0 * dir;
        nearestPoints[1] = segCenter + s1 * segDir;

        return std::make_pair(nearestPoints, distance);
    }
}
