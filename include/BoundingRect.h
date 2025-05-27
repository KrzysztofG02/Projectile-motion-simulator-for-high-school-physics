#ifndef BOUNDINGRECT_H
#define BOUNDINGRECT_H

#include <SFML/System/Vector2.hpp>


namespace gs
{

template<typename T>
struct BoundingRect
{
    T bottom;
    T left;
    T right;
    T top;

    constexpr BoundingRect(T const &b = 0, T const &l = 0, T const &r = 0, T const &t = 0):
        bottom(b > t ? b : t), left(l), right(r > l ? r : l), top(t) {}
    ~BoundingRect() = default;

    constexpr T getWidth() const
    {
        return right - left;
    }
    constexpr T getHeight() const
    {
        return bottom - top;
    }

    constexpr sf::Vector2f getCenter() const
    {
        return sf::Vector2f((left + right) / 2.L, (top + bottom) / 2.L);
    }

    constexpr bool operator==(BoundingRect const &other) const
    {
        return bottom == other.bottom && left == other.left && right == other.right && top == other.top;
    }

    constexpr bool operator!=(const BoundingRect& other) const
    {
        return !(*this == other);
    }
};

}

#endif // BOUNDINGRECT_H
