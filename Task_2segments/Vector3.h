#pragma once
#include <type_traits>

//---------------------------------------------------------------------------------------
template <typename TFloat>
class Vector3
{
    static_assert(
        std::is_floating_point_v<TFloat>,
        "Vector3 requires floating point type"
    );

public:
    static const TFloat eps;

public:
    struct {
        TFloat X;
        TFloat Y;
        TFloat Z;
    };

public:
    Vector3();

    Vector3(TFloat value);

    Vector3(TFloat x, TFloat y, TFloat z);

    bool operator==(const Vector3<TFloat>& other) const;

    bool operator!=(const Vector3<TFloat>& other) const;

    Vector3<TFloat> operator+(const Vector3<TFloat>& other) const;

    Vector3<TFloat> operator-(const Vector3<TFloat>& other) const;

    Vector3<TFloat>& operator+=(const Vector3<TFloat>& other);

    Vector3<TFloat>& operator-=(const Vector3<TFloat>& other);

    template<typename TArg>
    Vector3<TFloat> operator*(TArg scale) const;

    template<typename TArg>
    Vector3<TFloat> operator/(TArg scale) const;

    template<typename TArg>
    Vector3<TFloat>& operator*=(TArg scale);

    template<typename TArg>
    Vector3<TFloat>& operator/=(TArg scale);

    bool IsValid() const;

    TFloat Size() const;

    TFloat SizeSquared() const;

    Vector3<TFloat>& Normalize();

    TFloat Dot(const Vector3<TFloat>& other) const;

    Vector3<TFloat> Cross(const Vector3<TFloat>& other) const;

public:
    static TFloat DotProduct(
        const Vector3<TFloat>& first,
        const Vector3<TFloat>& second
    );

    static Vector3<TFloat> CrossProduct(
        const Vector3<TFloat>& first,
        const Vector3<TFloat>& second
    );

    static TFloat TripleProduct(
        const Vector3<TFloat>& first,
        const Vector3<TFloat>& second,
        const Vector3<TFloat>& third
    );
};

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline Vector3<TFloat>::Vector3()
  :
    X(0.),
    Y(0.),
    Z(0.)
{}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline Vector3<TFloat>::Vector3
(
    TFloat value
) :
    X(value),
    Y(value),
    Z(value)
{}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline Vector3<TFloat>::Vector3
(
    TFloat x,
    TFloat y,
    TFloat z
) :
    X(x),
    Y(y),
    Z(z)
{}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline bool Vector3<TFloat>::operator==
(
    const Vector3<TFloat>& other
)
const
{
    return (
        std::abs(X - other.X) < eps &&
        std::abs(Y - other.Y) < eps &&
        std::abs(Z - other.Z) < eps
    );
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline bool Vector3<TFloat>::operator!=
(
    const Vector3<TFloat>& other
)
const
{
    return (
        std::abs(X - other.X) >= eps ||
        std::abs(Y - other.Y) >= eps ||
        std::abs(Z - other.Z) >= eps
    );
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline Vector3<TFloat> Vector3<TFloat>::operator+
(
    const Vector3<TFloat>& other
)
const
{
    return {
        X + other.X,
        Y + other.Y,
        Z + other.Z
    };
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline Vector3<TFloat> Vector3<TFloat>::operator-
(
    const Vector3<TFloat>& other
)
const
{
    return {
        X - other.X,
        Y - other.Y,
        Z - other.Z
    };
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline Vector3<TFloat>& Vector3<TFloat>::operator+=
(
    const Vector3<TFloat>& other
)
{
    X += other.X;
    Y += other.Y;
    Z += other.Z;

    return *this;
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline Vector3<TFloat>& Vector3<TFloat>::operator-=
(
    const Vector3<TFloat>& other
)
{
    X -= other.X;
    Y -= other.Y;
    Z -= other.Z;

    return *this;
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
    template<typename TArg>
Vector3<TFloat> Vector3<TFloat>::operator*
(
    TArg scale
)
const
{
    static_assert(
        std::is_arithmetic_v<TArg>,
        "Vector3 scaling requires arithmetic type"
    );

    return {
        X * scale,
        Y * scale,
        Z * scale
    };
}

//---------------------------------------------------------------------------------------
template<typename TFloat, typename TArg>
Vector3<TFloat> operator*
(
    TArg scale,
    Vector3<TFloat> vector
)
{
    static_assert(
        std::is_arithmetic_v<TArg>,
        "Vector3 scaling requires arithmetic type"
    );

    return {
        vector.X * scale,
        vector.Y * scale,
        vector.Z * scale
    };
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
    template<typename TArg>
Vector3<TFloat> Vector3<TFloat>::operator/
(
    TArg scale
)
const
{
    static_assert(
        std::is_arithmetic_v<TArg>,
        "Vector3 scaling requires arithmetic type"
    );

    return {
        X / scale,
        Y / scale,
        Z / scale
    };
}

//---------------------------------------------------------------------------------------
template<typename TFloat, typename TArg>
Vector3<TFloat> operator/
(
    TArg scale,
    Vector3<TFloat> vector
)
{
    static_assert(
        std::is_arithmetic_v<TArg>,
        "Vector3 scaling requires arithmetic type"
        );

    return {
        vector.X / scale,
        vector.Y / scale,
        vector.Z / scale
    };
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
    template<typename TArg>
Vector3<TFloat>& Vector3<TFloat>::operator*=
(
    TArg scale
)
{
    static_assert(
        std::is_arithmetic_v<TArg>,
        "Vector3 scaling requires arithmetic type"
    );

    X *= scale;
    Y *= scale;
    Z *= scale;

    return *this;
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
    template<typename TArg>
Vector3<TFloat>& Vector3<TFloat>::operator/=
(
    TArg scale
)
{
    static_assert(
        std::is_arithmetic_v<TArg>,
        "Vector3 scaling requires arithmetic type"
    );

    X /= scale;
    Y /= scale;
    Z /= scale;

    return *this;
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
bool Vector3<TFloat>::IsValid() const
{
    return (
        !isnan(X) &&
        !isnan(Y) &&
        !isnan(Z) &&
        !isinf(X) &&
        !isinf(Y) &&
        !isinf(Z)
    );
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
TFloat Vector3<TFloat>::Size() const
{
    return sqrt(SizeSquared());
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
TFloat Vector3<TFloat>::SizeSquared() const
{
    if (!IsValid())
        return NAN;

    return (
        X * X +
        Y * Y +
        Z * Z
    );
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
Vector3<TFloat>& Vector3<TFloat>::Normalize()
{
    return *this /= Size();
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline TFloat Vector3<TFloat>::Dot
(
    const Vector3<TFloat>& other
)
const
{
    return (
        X * other.X +
        Y * other.Y +
        Z * other.Z
    );
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline Vector3<TFloat> Vector3<TFloat>::Cross
(
    const Vector3<TFloat>& other
)
const
{
    return {
        Y * other.Z - Z * other.Y,
        Z * other.X - X * other.Z,
        X * other.Y - Y * other.X
    };
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline TFloat Vector3<TFloat>::DotProduct
(
    const Vector3<TFloat>& first,
    const Vector3<TFloat>& second
)
{
    return first.Dot(second);
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline Vector3<TFloat> Vector3<TFloat>::CrossProduct
(
    const Vector3<TFloat>& first,
    const Vector3<TFloat>& second
)
{
    return first.Cross(second);
}

//---------------------------------------------------------------------------------------
template<typename TFloat>
inline TFloat Vector3<TFloat>::TripleProduct
(
    const Vector3<TFloat>& first,
    const Vector3<TFloat>& second,
    const Vector3<TFloat>& third
)
{
    return first.Dot(second.Cross(third));
}

//---------------------------------------------------------------------------------------
using Vector3F = Vector3<float>;
using Vector3D = Vector3<double>;

//---------------------------------------------------------------------------------------
const float  Vector3F::eps = 1e-7f;
const double Vector3D::eps = 1e-15;
