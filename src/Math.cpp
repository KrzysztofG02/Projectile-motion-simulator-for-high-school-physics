#include "Math.h"


template <>
mpf_class gs::toDegrees(mpf_class const &radianAngle)
{
    return radianAngle * GMP_180_TO_PI;
}


template <>
mpf_class gs::toRadians(mpf_class const &degreeAngle)
{
    return degreeAngle * GMP_PI_TO_180;
}


sf::Vector2f gs::getRotatedPointAroundOrigin(sf::Vector2f const &point, sf::Vector2f const &origin, float const radianAngle)
{
    return sf::Vector2f((point.x - origin.x) * cos(radianAngle) - (point.y - origin.y) * sin(radianAngle) + origin.x,
                        (point.x - origin.x) * sin(radianAngle) + (point.y - origin.y) * cos(radianAngle) + origin.y);
}


mpf_class gs::mpf_sin(mpf_class const &x, unsigned short const terms)
{
    mpf_class result(0, 256), xPower = x;
    mpz_class fact(1);

    for(unsigned short n = 0, m = 1; n < terms; ++n)
    {
        result += (n % 2 == 0 ? xPower : -xPower) / mpf_class(fact);
        xPower *= x * x;

        ++m;
        fact *= m;
        ++m;
        fact *= m;
    }

    return result;
}

mpf_class gs::mpf_cos(mpf_class const &x, unsigned int const terms)
{
    mpf_class result(0, 256), xPower(1, 256);
    mpz_class fact(1);

    for(unsigned short n = 0, m = 0; n < terms; ++n)
    {
        result += (n % 2 == 0 ? xPower : -xPower) / mpf_class(fact);
        xPower *= x * x;

        ++m;
        fact *= m;
        ++m;
        fact *= m;
    }

    return result;
}

mpf_class gs::mpf_atan(mpf_class const &x, unsigned int const terms)
{
    if(x < -1)
    {
        return -gs::GMP_HALF_PI - mpf_atan(1 / x, terms);
    }

    if(x > 1)
    {
        return gs::GMP_HALF_PI - mpf_atan(1 / x, terms);
    }

    mpf_class result("0.00", 256);
    mpf_class xPower = x;

    for(unsigned short n = 0, d = 1; n < terms; ++n, d += 2)
    {
        result += (n % 2 == 0 ? xPower : -xPower) / mpf_class(d);
        xPower *= x * x;
    }

    return result;
}
