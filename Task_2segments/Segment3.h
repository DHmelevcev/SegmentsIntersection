#pragma once
#include "Vector3.h"
#include <algorithm>

#ifdef _LOG
#include <iostream>
#endif // _LOG

//---------------------------------------------------------------------------------------
template <typename TFloat>
class Segment3
{
public:
    struct {
        Vector3<TFloat> Start;
        Vector3<TFloat> End;
    };

public:
    Segment3() = default;

    Segment3(const Vector3<TFloat>& end);

    Segment3(
        const Vector3<TFloat>& start,
        const Vector3<TFloat>& end
    );

    TFloat Size() const;

    TFloat SizeSquared() const;

    Vector3<TFloat> ToVector() const;

    Segment3<TFloat> ToAABB() const;

    bool IsPoint() const;

    bool AABBOverlap(const Vector3<TFloat>& point) const;

    bool AABBOverlap(const Segment3<TFloat>& other) const;

    bool Intersect(const Vector3<TFloat>& point) const;

    Vector3<TFloat> Intersection(const Vector3<TFloat>& point) const;

    Vector3<TFloat> Intersection(const Segment3<TFloat>& other) const;
};

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline Segment3<TFloat>::Segment3
(
    const Vector3<TFloat>& end
) :
    End(end)
{}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline Segment3<TFloat>::Segment3
(
    const Vector3<TFloat>&start,
    const Vector3<TFloat>&end
) :
    Start(start),
    End(end)
{}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline TFloat Segment3<TFloat>::Size() const
{
    return sqrt(SizeSquared());
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline TFloat Segment3<TFloat>::SizeSquared() const
{
    return ToVector().SizeSquared();
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline Vector3<TFloat> Segment3<TFloat>::ToVector() const
{
    return End - Start;
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline Segment3<TFloat> Segment3<TFloat>::ToAABB() const
{
    auto minmaxX = std::minmax(Start.X, End.X);
    auto minmaxY = std::minmax(Start.Y, End.Y);
    auto minmaxZ = std::minmax(Start.Z, End.Z);

    return Segment3<TFloat>(
        {
            minmaxX.first,
            minmaxY.first,
            minmaxZ.first,
        },
        {
            minmaxX.second,
            minmaxY.second,
            minmaxZ.second,
        }
    );
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline bool Segment3<TFloat>::IsPoint() const
{
    return Start == End;
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline bool Segment3<TFloat>::AABBOverlap
(
    const Vector3<TFloat>& point
)
const
{
    Segment3<TFloat> AABB = ToAABB();

    return (
        point.X >= AABB.Start.X &&
        point.Y >= AABB.Start.Y &&
        point.Z >= AABB.Start.Z &&
        point.X <= AABB.End  .X &&
        point.Y <= AABB.End  .Y &&
        point.Z <= AABB.End  .Z
    );
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline bool Segment3<TFloat>::AABBOverlap
(
    const Segment3<TFloat>& other
)
const
{
    Segment3<TFloat> first  = this->ToAABB();
    Segment3<TFloat> second = other.ToAABB();

    return (
        first .Start.X <= second.End.X &&
        first .Start.Y <= second.End.Y &&
        first .Start.Z <= second.End.Z &&
        second.Start.X <= first .End.X &&
        second.Start.Y <= first .End.Y &&
        second.Start.Z <= first .End.Z
    );
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline bool Segment3<TFloat>::Intersect
(
    const Vector3<TFloat>& point
)
const
{
#ifdef _LOG
    std::cout << "Segment-point intersection:\n";
#endif // _LOG

    // One or more pairs of projections do not intersect
    if (!this->AABBOverlap(point)) {
#ifdef _LOG
        std::cout << "Point is outside the bounding box\n";
#endif // _LOG
        return false;
    }

    // Not on the same line
    if (std::abs(Vector3<TFloat>::CrossProduct(
        this->ToVector(),
        point - this->Start
    ).SizeSquared()) > Vector3<TFloat>::eps)
    {
#ifdef _LOG
        std::cout << "Point is not on the line\n";
#endif // _LOG
       return false;
    }

    return true;
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline Vector3<TFloat> Segment3<TFloat>::Intersection
(
    const Vector3<TFloat>& point
)
const
{
    return Intersect(point) ? point : Vector3<TFloat>(NAN);
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline Vector3<TFloat> Segment3<TFloat>::Intersection
(
    const Segment3<TFloat>& other
)
const
{
#ifdef _LOG
    std::cout << "Segments intersection:\n";
#endif // _LOG

    if (this->IsPoint()) {
#ifdef _LOG
        std::cout << "Segment 1 is point\n";
#endif // _LOG
        return other.Intersection(this->Start);
    }

    if (other.IsPoint()) {
#ifdef _LOG
        std::cout << "Segment 2 is point\n";
#endif // _LOG
        return this->Intersection(other.Start);
    }

    // One or more pairs of projections do not intersect
    if (!this->AABBOverlap(other))
    {
#ifdef _LOG
        std::cout << "Bounding boxes do not overlap\n";
#endif // _LOG
        return Vector3<TFloat>(NAN);
    }

    Vector3<TFloat> thisV  = this->ToVector();
    Vector3<TFloat> otherV = other.ToVector();
    {
        Vector3<TFloat> crossV = thisV.Cross(otherV);
        Vector3<TFloat> startDist = this->Start - other.Start;

        // Skew lines
        if (std::abs(Vector3<TFloat>::DotProduct(
            startDist,
            crossV
        )) > Vector3<TFloat>::eps)
        {
#ifdef _LOG
            std::cout << "Skew lines\n";
#endif // _LOG
            return Vector3<TFloat>(NAN);
        }
        // => on the same plane

        // Parallel lines or the same line
        if (crossV.SizeSquared() < Vector3<TFloat>::eps)
        {
            // Parallel lines
            if (std::abs(Vector3<TFloat>::CrossProduct(
                startDist,
                thisV
            ).SizeSquared()) > Vector3<TFloat>::eps)
            {
#ifdef _LOG
                std::cout << "Parallel lines\n";
#endif // _LOG
                return Vector3<TFloat>(NAN);
            }
            // => same line

#ifdef _LOG
            std::cout << "Same line\n";
#endif // _LOG
            return (
                this->AABBOverlap(other.Start) ?
                other.Start : other.End
                );
        }
        // => lines intersect
    }

    TFloat Vector3<TFloat>::* U = &Vector3<TFloat>::X;
    TFloat Vector3<TFloat>::* V = &Vector3<TFloat>::Y;

    while (std::abs(thisV .X) < Vector3<TFloat>::eps ||
           std::abs(otherV.Y) < Vector3<TFloat>::eps)
    {
        if (std::abs(thisV .Y) > Vector3<TFloat>::eps &&
            std::abs(otherV.Z) > Vector3<TFloat>::eps )
        {
            U = &Vector3<TFloat>::Y;
            V = &Vector3<TFloat>::Z;
            break;
        }

        if (std::abs(thisV .Z) > Vector3<TFloat>::eps &&
            std::abs(otherV.X) > Vector3<TFloat>::eps )
        {
            U = &Vector3<TFloat>::Z;
            V = &Vector3<TFloat>::X;
            break;
        }

        if (std::abs(thisV .Y) > Vector3<TFloat>::eps &&
            std::abs(otherV.X) > Vector3<TFloat>::eps )
        {
            U = &Vector3<TFloat>::Y;
            V = &Vector3<TFloat>::X;
            break;
        }

        if (std::abs(thisV .Z) > Vector3<TFloat>::eps &&
            std::abs(otherV.Y) > Vector3<TFloat>::eps )
        {
            U = &Vector3<TFloat>::Z;
            V = &Vector3<TFloat>::Y;
            break;
        }

        if (std::abs(thisV .X) > Vector3<TFloat>::eps &&
            std::abs(otherV.Z) > Vector3<TFloat>::eps )
        {
            U = &Vector3<TFloat>::X;
            V = &Vector3<TFloat>::Z;
            break;
        }
    }

    double t = (
        thisV.*U * (other.Start.*V - this->Start.*V) -
        thisV.*V * (other.Start.*U - this->Start.*U)
        ) / (
        thisV.*V * otherV.*U - otherV.*V * thisV.*U
    );

    double k = (
        otherV.*U * t + other.Start.*U - this->Start.*U
        ) / (
        thisV.*U
    );

#ifdef _LOG
    if (U == &Vector3<TFloat>::X)
        std::cout << "U: X\n";
    else if (U == &Vector3<TFloat>::Y)
        std::cout << "U: Y\n";
    else
        std::cout << "U: Z\n";

    if (V == &Vector3<TFloat>::X)
        std::cout << "V: X\n";
    else if (V == &Vector3<TFloat>::Y)
        std::cout << "V: Y\n";
    else
        std::cout << "V: Z\n";

    std::cout << "t: " << t << " " << "k: " << k << "\n";
#endif // _LOG

    if (t < 0. || 1. < t || k < 0. || 1. < k)
        return Vector3<TFloat>(NAN);

    return other.Start + t * otherV;
}

//---------------------------------------------------------------------------------------
using Segment3F = Segment3<float>;
using Segment3D = Segment3<double>;
