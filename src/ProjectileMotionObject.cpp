#include "ProjectileMotionObject.h"

#include "App.h"

#include <cmath>
#include <iostream>

using namespace std;


gs::ProjectileMotionObject::ProjectileMotionObject(mpf_class const &acceleration,
                                                   sf::Vector2<mpf_class> zeroPoint,
                                                   float const radius,
                                                   sf::Color const color,
                                                   sf::Color const outlineColor):
    gs::PhysicalObject<mpf_class>(false),

    mAcc(acceleration),
    mScale(1, 1),
    mZeroPoint(zeroPoint),

    mH0(0),
    mV0(0),
    mAngle0(gs::toRadians(mpf_class("45.0"))),

    mElapsedTime_s("0.00"),

    mObject(radius, 24)
{
    mObject.setOutlineThickness(-2.f);
    mObject.setOutlineColor(outlineColor);

    mObject.setOrigin(mObject.getOrigin() + sf::Vector2f(radius, radius));
    this->setPosition(mZeroPoint);
    mObject.setFillColor(color);

    this->calcAllResultantValues();
}



void gs::ProjectileMotionObject::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mObject, states);
}


void gs::ProjectileMotionObject::reset()
{
    mElapsedTime_s = 0;

    this->setPosition(mPosition0);

         mDistance = 0;
                mH = mH0;
                mV = mV0;
               mVy = mVy0;
          mVySense = mVy0Sense;
            mAngle = mAngle0;

    mEquilibrium = (mH == 0 && mVy == 0);
}


void gs::ProjectileMotionObject::affectObject(mpf_class const &secs)
{
    if(!mEquilibrium)
    {
        mElapsedTime_s += secs;

        if(mElapsedTime_s >= mTf_s)
        {
            mElapsedTime_s = mTf_s;
            mEquilibrium = true;
        }

        this->setPosition(this->getPositionAfterTime(mElapsedTime_s));

         mDistance = this->getDistanceAfterTime(mElapsedTime_s);
                mH = this->getHAfterTime(mElapsedTime_s);
                mV = this->getVAfterTime(mElapsedTime_s);
               mVy = this->getVyAfterTime(mElapsedTime_s);
          mVySense = this->getVySenseAfterTime(mElapsedTime_s);
            mAngle = this->getAngleAfterTime(mElapsedTime_s);
    }
}


mpf_class gs::ProjectileMotionObject::getAcc() const
{
    return mAcc;
}

sf::Vector2<mpf_class> const & gs::ProjectileMotionObject::getScale() const
{
    return mScale;
}

sf::Vector2<mpf_class> gs::ProjectileMotionObject::getZeroPoint() const
{
    return mZeroPoint;
}

mpf_class gs::ProjectileMotionObject::getH0() const
{
    return mH0;
}

mpf_class gs::ProjectileMotionObject::getV0() const
{
    return mV0;
}

mpf_class gs::ProjectileMotionObject::getAngle0() const
{
    return mAngle0;
}

gs::VectorSense gs::ProjectileMotionObject::getVxSense() const
{
    return mVxSense;
}

gs::VectorSense gs::ProjectileMotionObject::getVy0Sense() const
{
    if(mVy0 > 0)
    {
        return gs::VectorSense::up;
    }

    if(mVy0 < 0)
    {
        return gs::VectorSense::down;
    }

    return gs::VectorSense::none;
}


mpf_class gs::ProjectileMotionObject::getElapsedTime_s() const
{
    return mElapsedTime_s;
}

sf::Vector2<mpf_class> gs::ProjectileMotionObject::getPosition0() const
{
    return mPosition0;
}

sf::Vector2<mpf_class> gs::ProjectileMotionObject::getFixedPosition0() const
{
    return sf::Vector2<mpf_class>(mPosition0.x - mZeroPoint.x, mZeroPoint.y - mPosition0.y);
}

mpf_class gs::ProjectileMotionObject::getMaxH() const
{
    return mMaxH;
}

mpf_class gs::ProjectileMotionObject::getReach() const
{
    return mReach;
}

mpf_class gs::ProjectileMotionObject::getRaiseTime() const
{
    return mTr_s;
}

mpf_class gs::ProjectileMotionObject::getDescentTime() const
{
    return mTd_s;
}

mpf_class gs::ProjectileMotionObject::getFlightTime() const
{
    return mTf_s;
}

mpf_class gs::ProjectileMotionObject::getVx() const
{
    return mVx;
}

mpf_class gs::ProjectileMotionObject::getVy0() const
{
    return mVy0;
}


sf::Vector2<mpf_class> gs::ProjectileMotionObject::getCurrentPosition() const
{
    return mCurrentPosition;
}

sf::Vector2<mpf_class> gs::ProjectileMotionObject::getFixedCurrentPosition() const
{
    return sf::Vector2<mpf_class>(mCurrentPosition.x - mZeroPoint.x, mZeroPoint.y - mCurrentPosition.y);
}

mpf_class gs::ProjectileMotionObject::getCurrentDistance() const
{
    return mDistance;
}

mpf_class gs::ProjectileMotionObject::getCurrentH() const
{
    return mH;
}

mpf_class gs::ProjectileMotionObject::getCurrentV() const
{
    return mV;
}

gs::VectorSense gs::ProjectileMotionObject::getCurrentVySense() const
{
    return mVySense;
}

mpf_class gs::ProjectileMotionObject::getCurrentVy() const
{
    return mVy;
}

mpf_class gs::ProjectileMotionObject::getCurrentAngle() const
{
    return mAngle;
}


sf::Vector2<mpf_class> gs::ProjectileMotionObject::getPositionAfterTime(mpf_class const &secs) const
{
    if(secs >= mTf_s)
    {
        mpf_class const &&xOffset = mReach * mScale.x;

        return sf::Vector2<mpf_class>(mPosition0.x + xOffset, mZeroPoint.y);
    }

    return mPosition0 + sf::Vector2<mpf_class>(mScale.x * secs * mVx, mScale.y * secs * (mAcc * secs / 2 - mVy0));
}

sf::Vector2<mpf_class> gs::ProjectileMotionObject::getFixedPositionAfterTime(mpf_class const &seconds) const
{
    sf::Vector2<mpf_class> const posAfterTime = this->getPositionAfterTime(seconds);

    return sf::Vector2<mpf_class>(posAfterTime.x - mZeroPoint.x, mZeroPoint.y - posAfterTime.y);
}

mpf_class gs::ProjectileMotionObject::getDistanceAfterTime(mpf_class const &secs) const
{
    if(secs > mTf_s)
    {
        return mReach;
    }

    if(mAngle0 == mpf_class("90.00") || mAngle0 == mpf_class("-90.00"))
    {
        return mpf_class("0.00");
    }

    return mVx * secs;
}

mpf_class gs::ProjectileMotionObject::getVAfterTime(mpf_class secs) const
{
    if(secs > mTf_s)
    {
        secs = mTf_s;
    }

    mpf_class const Vy = this->getVyAfterTime(secs);

    if(mVx == 0 || Vy == 0)
    {
        return Vy + mVx;
    }

    return Vy / gs::mpf_sin(this->getAngleAfterTime(secs));
}

mpf_class gs::ProjectileMotionObject::getHAfterTime(mpf_class const &secs) const
{
    if(secs > mTf_s)
    {
        return mpf_class("0.00");
    }

    return mH0 + secs * (mVy0 - mAcc * secs / 2);
}

gs::VectorSense gs::ProjectileMotionObject::getVySenseAfterTime(mpf_class secs) const
{
    if(secs > mTf_s)
    {
        secs = mTf_s;
    }

    mpf_class const &&Vy = this->getVyAfterTime(secs);

    if(Vy > 0)
    {
        return gs::VectorSense::up;
    }

    if(Vy < 0)
    {
        return gs::VectorSense::down;
    }

    return gs::VectorSense::none;
}

mpf_class gs::ProjectileMotionObject::getVxAfterTime(mpf_class const &secs) const
{
    return mVx;
}

mpf_class gs::ProjectileMotionObject::getVyAfterTime(mpf_class secs) const
{
    if(secs > mTf_s)
    {
        secs = mTf_s;
    }

    mpf_class const &&Vy = mVy0 - secs * mAcc;

    if(Vy > gs::App::terminalV)
    {
        return gs::App::terminalV;
    }

    if(Vy < -gs::App::terminalV)
    {
        return -gs::App::terminalV;
    }

    return Vy;
}

mpf_class gs::ProjectileMotionObject::getAngleAfterTime(mpf_class secs) const
{
    if(secs > mTf_s)
    {
        secs = mTf_s;
    }

    if(secs == mTf_s && mH0 == 0)
    {
        return -mAngle0;
    }

    mpf_class const &&Vy = this->getVyAfterTime(secs);

    if(mVx == 0)
    {
        if(Vy > 0)
        {
            return mPlus90degInRadians;
        }

        if(Vy == 0)
        {
            return mpf_class("0.00");
        }

        return mMinus90degInRadians;
    }

    if(Vy == 0)
    {
        return mpf_class("0.00");
    }

    return gs::mpf_atan(Vy / mVx, mTaylorSeriesTermsForAtan);
}


float gs::ProjectileMotionObject::getRadius() const
{
    return mObject.getRadius();
}

sf::Color gs::ProjectileMotionObject::getColor() const
{
    return mObject.getFillColor();
}

sf::Color gs::ProjectileMotionObject::getOutlineColor() const
{
    return mObject.getOutlineColor();
}


void gs::ProjectileMotionObject::setPosition(sf::Vector2<mpf_class> const &position)
{
    mCurrentPosition = position;
    mObject.setPosition(sf::Vector2f(mCurrentPosition.x.get_d(), mCurrentPosition.y.get_d()));
}

void gs::ProjectileMotionObject::setScale(sf::Vector2<mpf_class> const &scale)
{
    if(scale != mScale && scale.x > 0 && scale.y > 0)
    {
        sf::Vector2<mpf_class> const &&newOldDimsScaleRatio = {scale.x / mScale.x, scale.y / mScale.y};

        sf::Vector2<mpf_class> const distance = {
            newOldDimsScaleRatio.x * (this->getCurrentPosition().x - mPosition0.x),
            newOldDimsScaleRatio.y * (mPosition0.y - this->getCurrentPosition().y)};

        mScale = scale;

        this->calcPosition0();
        this->setPosition(sf::Vector2<mpf_class>(mPosition0.x + distance.x, mPosition0.y - distance.y));
    }
}

void gs::ProjectileMotionObject::setZeroPoint(sf::Vector2<mpf_class> const &zeroPoint)
{
    mZeroPoint = zeroPoint;
    this->calcPosition0();

    this->reset();
}

void gs::ProjectileMotionObject::setAcc(mpf_class const &acc)
{
    mAcc = acc;

    this->calcAllResultantValuesExceptPosition0();

    this->reset();
}

void gs::ProjectileMotionObject::setH0(mpf_class const &h0)
{
    if(h0 < 0)
    {
        mH0 = 0;
    }
    else
    {
        mH0 = h0;
    }

    this->calcAllResultantValues();

    this->reset();
}

void gs::ProjectileMotionObject::setV0(mpf_class const &v0)
{
    mV0 = v0;

    this->calcAllResultantValuesExceptPosition0();

    this->reset();
}

void gs::ProjectileMotionObject::setAngle0(mpf_class const &angle0)
{
    mAngle0 = angle0;

    this->calcAllResultantValuesExceptPosition0();

    this->reset();
}


void gs::ProjectileMotionObject::setRadius(float const radius)
{
    mObject.setRadius(radius);
}

void gs::ProjectileMotionObject::setColor(sf::Color const color)
{
    mObject.setFillColor(color);
}

void gs::ProjectileMotionObject::setOutlineColor(sf::Color const outlineColor)
{
    mObject.setOutlineColor(outlineColor);
}


void gs::ProjectileMotionObject::calcPosition0()
{
    mpf_class const &&H0 = mH0 * mScale.y;

    mPosition0 = mZeroPoint - sf::Vector2<mpf_class>(0.f, H0);
}

void gs::ProjectileMotionObject::calcVx()
{
    if(mV0 == 0 || mAngle0 == mPlus90degInRadians || mAngle0 == mMinus90degInRadians)
    {
        mVx = mpf_class("0.00");
    }
    else
    {
        mVx = mV0 * gs::mpf_cos(mAngle0);
    }
}

void gs::ProjectileMotionObject::calcVxSense()
{
    if(mVx > 0)
    {
        mVxSense = gs::VectorSense::right;
    }
    else if(mVx < 0)
    {
        mVxSense = gs::VectorSense::left;
    }
    else
    {
        mVxSense = gs::VectorSense::none;
    }
}

void gs::ProjectileMotionObject::calcVy0()
{
    if(mAngle0 == 0)
    {
        mVy0 = mpf_class("0.00");
    }
    else
    {
        mVy0 = mV0 * gs::mpf_sin(mAngle0);
    }
}

void gs::ProjectileMotionObject::calcVy0Sense()
{
    if(mVy > 0)
    {
        mVy0Sense = gs::VectorSense::up;
    }
    else if(mVy < 0)
    {
        mVy0Sense = gs::VectorSense::down;
    }
    else
    {
        mVy0Sense = gs::VectorSense::none;
    }
}

void gs::ProjectileMotionObject::calcMaxH()
{
    mMaxH = mH0;

    if(mVy0 > 0)
    {
        mMaxH += mVy0 * mVy0 / 2 / mAcc;
    }
}

void gs::ProjectileMotionObject::calcRaiseTime()
{
    mTr_s = 0;

    if(mVy0 > 0)
    {
        mTr_s += mVy0 / mAcc;
    }
}

void gs::ProjectileMotionObject::calcDescentTime()
{
    mTd_s = mVy0 * mVy0 + mAcc * mH0 * 2;
    mpf_sqrt(mTd_s.get_mpf_t(), mTd_s.get_mpf_t());

    if(mVy0 < 0)
    {
        mTd_s += mVy0;
    }

    mTd_s /= mAcc;
}

void gs::ProjectileMotionObject::calcFlightTime()
{
    mTf_s = mVy0 * mVy0 + mAcc * mH0 * 2;
    mpf_sqrt(mTf_s.get_mpf_t(), mTf_s.get_mpf_t());
    mTf_s = (mTf_s + mVy0) / mAcc;
}

void gs::ProjectileMotionObject::calcReach()
{
    mReach = mVx * mTf_s;
}

void gs::ProjectileMotionObject::calcAllResultantValuesExceptPosition0()
{
    this->calcVx();
    this->calcVxSense();
    this->calcVy0();
    this->calcVy0Sense();
    this->calcMaxH();
    this->calcRaiseTime();
    this->calcDescentTime();
    this->calcFlightTime();
    this->calcReach();
}

void gs::ProjectileMotionObject::calcAllResultantValues()
{
    this->calcPosition0();
    this->calcAllResultantValuesExceptPosition0();
}
