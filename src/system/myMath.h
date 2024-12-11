#pragma once
#include <ostream>
#include <limits>
#include <cmath>
#include <QDebug>
template <class Number>
class CPoint2
{
public:
    Number x, y;
    CPoint2<Number>(const Number _x = std::numeric_limits<Number>::min(), const Number _y = std::numeric_limits<Number>::min()) : x(_x), y(_y) {}

    CPoint2<Number>(const CPoint2<Number> &p) : x(p.x), y(p.y) {}

    CPoint2<Number> operator+(const CPoint2<Number> &p) const
    {
        return CPoint2<Number>(x + p.x, y + p.y);
    }

    CPoint2<Number> operator-(const CPoint2<Number> &p) const
    {
        return CPoint2<Number>(x - p.x, y - p.y);
    }

    CPoint2<Number> operator-() const
    {
        return CPoint2<Number>(-x, -y);
    }

    CPoint2<Number> operator*(const CPoint2<Number> &p) const
    {
        return CPoint2<Number>(x * p.x, y * p.y);
    }

    CPoint2<Number> operator/(const CPoint2<Number> &p) const
    {
        return CPoint2<Number>(x / p.x, y / p.y);
    }

    CPoint2<Number> &operator+=(const CPoint2<Number> &p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }

    CPoint2<Number> &operator-=(const CPoint2<Number> &p)
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    CPoint2<Number> &operator*=(const CPoint2<Number> &p)
    {
        x *= p.x;
        y *= p.y;
        return *this;
    }

    CPoint2<Number> &operator/=(const CPoint2<Number> &p)
    {
        x /= p.x;
        y /= p.y;
        return *this;
    }
    //模平方
    Number vSize2() const
    {
        return x * x + y * y;
    }

    //模
    Number vSize() const
    {
        return std::sqrt(vSize2());
    }
    template <typename num_t, typename = typename std::enable_if<std::is_arithmetic<num_t>::value, num_t>::type>
    CPoint2<Number> operator*(const num_t i) const
    {
        return CPoint2<Number>(x * i, y * i);
    }

    template <typename num_t, typename = typename std::enable_if<std::is_arithmetic<num_t>::value, num_t>::type>
    CPoint2<Number> operator/(const num_t i) const
    {
        return CPoint2<Number>(x / i, y / i);
    }

    template <typename num_t, typename = typename std::enable_if<std::is_arithmetic<num_t>::value, num_t>::type>
    CPoint2<Number> &operator*=(const num_t i)
    {
        x *= i;
        y *= i;
        return *this;
    }

    template <typename num_t, typename = typename std::enable_if<std::is_arithmetic<num_t>::value, num_t>::type>
    CPoint2<Number> &operator/=(const num_t i)
    {
        x /= i;
        y /= i;
        return *this;
    }

    bool operator==(const CPoint2<Number> &p) const
    {
        return x == p.x && y == p.y;
    }

    bool operator!=(const CPoint2<Number> &p) const
    {
        return x != p.x || y != p.y;
    }
    friend QDebug &operator<<(QDebug out, const CPoint2<Number> &info)
    {
        out << "CPoint2(" << info.x << "," << info.y << ")";
        return out;
    }
};

using CPoint2d = CPoint2<double>;