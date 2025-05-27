#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

#include <gmpxx.h>

#include <SFML/System/Vector2.hpp>

#include <cmath>


namespace gs
{
    constexpr long double LD_PI = 3.14159265358979323846264338327950L;
    constexpr long double LD_180_TO_PI = 180.0L / LD_PI;
    constexpr long double LD_PI_TO_180 = LD_PI / 180.0L;

    mpf_class const GMP_PI("3.14159265358979323846264338327950", 512);
    mpf_class const GMP_180_TO_PI = [] {mpf_class result; mpf_div(result.get_mpf_t(), mpf_class("180.00", 512).get_mpf_t(), GMP_PI.get_mpf_t()); return result;} ();
    mpf_class const GMP_PI_TO_180 = [] {mpf_class result; mpf_div(result.get_mpf_t(), GMP_PI.get_mpf_t(), mpf_class("180.00", 512).get_mpf_t()); return result;} ();
    mpf_class const GMP_HALF_PI = GMP_PI / 2;


    template <typename T>
    T toDegrees(T const &radianAngle)
    {
        return radianAngle * static_cast<T>(LD_180_TO_PI);
    }

    template <>
    mpf_class toDegrees(mpf_class const &radianAngle);


    template <typename T>
    T toRadians(T const &degreeAngle)
    {
        return degreeAngle * static_cast<T>(LD_PI_TO_180);
    }

    template <>
    mpf_class toRadians(mpf_class const &degreeAngle);


    sf::Vector2f getRotatedPointAroundOrigin(sf::Vector2f const &point, sf::Vector2f const &origin, float const radianAngle);


    template <typename T>
    short sgn(T val)
    {
        return   static_cast<short>(static_cast<T>(0) < val)
               - static_cast<short>(val < static_cast<T>(0));
    }


    template<std::size_t N, typename T>
    constexpr std::size_t countOf(T const (&arr)[N])
    {
        return N;
    }


    mpf_class mpf_sin(mpf_class const &x, unsigned short const terms = 50);
    mpf_class mpf_cos(mpf_class const &x, unsigned int const terms = 50);
    mpf_class mpf_atan(mpf_class const &x, unsigned int const terms = 50);
}

#endif // MATH_H_INCLUDED
