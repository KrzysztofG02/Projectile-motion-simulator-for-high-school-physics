#ifndef PROJECTILEMOTIONOBJECT_H
#define PROJECTILEMOTIONOBJECT_H

#include "Math.h"
#include "PhysicalObject.h"
#include "VectorSense.h"

#include <gmpxx.h>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>


namespace gs
{

class ProjectileMotionObject final: public gs::PhysicalObject<mpf_class>
{
private:
    static constexpr unsigned short mTaylorSeriesTermsForAtan = 3000;
    mpf_class const mPlus90degInRadians = toRadians(mpf_class("90.00"));
    mpf_class const mMinus90degInRadians = toRadians(mpf_class("-90.00"));


private:
//initial values
    mpf_class mAcc;                     //m/s^2
    sf::Vector2<mpf_class> mScale;      //horizontal, vertical
    sf::Vector2<mpf_class> mZeroPoint;  //x - default horizontal coord, y - ground's vertical coord

    mpf_class mH0;      //m
    mpf_class mV0;      //m/s
    mpf_class mAngle0;  //range: <-pi/2, pi/2> [rad]
//initial values/

//result values
    sf::Vector2<mpf_class> mPosition0;
    mpf_class mMaxH;    //m
    mpf_class mReach;   //m
    mpf_class mTr_s;    //s
    mpf_class mTd_s;    //s
    mpf_class mTf_s;    //s
    mpf_class mVx;      //m/s
    gs::VectorSense mVxSense;
    mpf_class mVy0;     //m/s
    gs::VectorSense mVy0Sense;
//result values/

//time-varying values
    sf::Vector2<mpf_class> mCurrentPosition;
    mpf_class mElapsedTime_s;   //s; elapsed time, cleared before each simulation
    mpf_class mDistance;        //m
    mpf_class mV;               //m/s
    mpf_class mH;               //m
    mpf_class mVy;              //m/s
    gs::VectorSense mVySense;
    mpf_class mAngle;           //rad
//time-varying values/

    sf::CircleShape mObject;


public:
    ProjectileMotionObject(mpf_class const &acceleration = mpf_class("10.00"),
                           sf::Vector2<mpf_class> zeroPoint = sf::Vector2<mpf_class>(),
                           float const radius = 10.f,
                           sf::Color const color = sf::Color::Transparent,
                           sf::Color const outlineColor = sf::Color(255, 165, 0, 160));
    ~ProjectileMotionObject() = default;

private:
    //from sf::Drawable
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    //from sf::Drawable/

public:
    void reset();

private:
    //from gs::PhysicalObject
    void affectObject(mpf_class const &secs) override;
    //from gs::PhysicalObject/

public:
    //getters
    mpf_class getAcc() const;
    sf::Vector2<mpf_class> const & getScale() const;
    sf::Vector2<mpf_class> getZeroPoint() const;

    mpf_class getH0() const;
    mpf_class getV0() const;
    mpf_class getAngle0() const;
    gs::VectorSense getVxSense() const;
    gs::VectorSense getVy0Sense() const;

    mpf_class getElapsedTime_s() const;
    sf::Vector2<mpf_class> getPosition0() const;
    sf::Vector2<mpf_class> getFixedPosition0() const;
    mpf_class getMaxH() const;
    mpf_class getReach() const;
    mpf_class getRaiseTime() const;
    mpf_class getDescentTime() const;
    mpf_class getFlightTime() const;
    mpf_class getVx() const;
    mpf_class getVy0() const;

    //current = since the last update
    sf::Vector2<mpf_class> getCurrentPosition() const;
    sf::Vector2<mpf_class> getFixedCurrentPosition() const;
    mpf_class getCurrentDistance() const;
    mpf_class getCurrentH() const;
    mpf_class getCurrentV() const;
    gs::VectorSense getCurrentVySense() const;
    mpf_class getCurrentVy() const;
    mpf_class getCurrentAngle() const;
    //current = since the last update//

    sf::Vector2<mpf_class> getPositionAfterTime(mpf_class const &secs) const;
    sf::Vector2<mpf_class> getFixedPositionAfterTime(mpf_class const &secs) const;
    mpf_class getDistanceAfterTime(mpf_class const &secs) const;
    mpf_class getHAfterTime(mpf_class const &secs) const;
    mpf_class getVAfterTime(mpf_class secs) const;
    gs::VectorSense getVySenseAfterTime(mpf_class secs) const;
    mpf_class getVxAfterTime(mpf_class const &secs) const;
    mpf_class getVyAfterTime(mpf_class secs) const;
    mpf_class getAngleAfterTime(mpf_class secs) const;

    float getRadius() const;
    sf::Color getColor() const;
    sf::Color getOutlineColor() const;
    //getters/

public:
    //setters
    void setScale(sf::Vector2<mpf_class> const &scale);
    void setZeroPoint(sf::Vector2<mpf_class> const &zeroPoint);

    void setAcc(mpf_class const &acc);
    void setH0(mpf_class const &h0);
    void setV0(mpf_class const &v0);
    void setAngle0(mpf_class const &angle0);

    void setRadius(float const radius);
    void setColor(sf::Color const color);
    void setOutlineColor(sf::Color const outlineColor);

private:
    void setPosition(sf::Vector2<mpf_class> const &position);
    //setters/

    //The call order is critical.
    void calcPosition0();
    void calcVx();
    void calcVxSense();
    void calcVy0();
    void calcVy0Sense();
    void calcMaxH();
    void calcRaiseTime();
    void calcDescentTime();
    void calcFlightTime();
    void calcReach();

    void calcAllResultantValuesExceptPosition0();
    void calcAllResultantValues();
    //The call order is critical./
};

}

#endif // PROJECTILEMOTIONOBJECT_H
