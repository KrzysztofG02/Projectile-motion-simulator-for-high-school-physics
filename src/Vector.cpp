#include "Vector.h"

#include <iostream>

using namespace std;


gs::Vector::Vector(sf::Vector2f const &pointOfApp,
                   float const units,
                   float const heightUnitsPerUnit,
                   float const deegreAngle,
                   unsigned short const thickness,
                   sf::Color const innerColor,
                   unsigned short const borderThickness,
                   bool const borderVisibility,
                   sf::Color const borderColor):
    Border(borderThickness > mMaxBorderThickness ? mMaxBorderThickness : borderThickness, borderVisibility, borderColor),
    mPointOfApp(pointOfApp),
    mArrow(mTotalVertices),
    mUnits(units < 0.f ? 0.f : units),
    mHeightUnitsPerUnit(heightUnitsPerUnit < 0.f ? 0.f : heightUnitsPerUnit),
    mDeegreAngle(deegreAngle),
    mThickness(thickness),
    mInnerColor(innerColor),
    mMinVectorLength(2 + ceil((mThickness / 2 + 2) * sqrt(3))),
    mMaxBorderThickness(mThickness / 2 - 1),
    name(mPointOfApp)
{
    name.setPosition(mPointOfApp);
    name.setScale(sf::Vector2f(mTitleScale, mTitleScale));

    this->setAllPoints();
    this->setInnerPointsColor();

    this->updateBorderColor();
    this->updateBorderVisibility();
}


void gs::Vector::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if(mUnits >= mMinUnitsToRender)
    {
        target.draw(&mArrow[0], mOuterBaseVerticesNum, sf::Quads, states);
        target.draw(&mArrow[0 + mOuterBaseVerticesNum], mOuterTipVerticesNum, sf::Triangles, states);

        if(mBorderVisibility)
        {
            target.draw(&mArrow[mAllOuterVerticesNum], mInnerBaseVerticesNum, sf::Quads, states);
            target.draw(&mArrow[mAllOuterVerticesNum + mInnerBaseVerticesNum], mInnerTipVerticesNum, sf::Triangles, states);
        }
    }
}


//getters
sf::Vector2f gs::Vector::getPointOfApp() const
{
    return mPointOfApp;
}

float gs::Vector::getUnits() const
{
    return mUnits;
}

float gs::Vector::getHeightUnitsPerUnit() const
{
    return mHeightUnitsPerUnit;
}

float gs::Vector::getDeegreAngle() const
{
    return mDeegreAngle;
}

sf::Color gs::Vector::getInnerColor() const
{
    return mInnerColor;
}

unsigned short gs::Vector::getMaxBorderThickness() const
{
    return mMaxBorderThickness;
}
//getters/


//setters
void gs::Vector::setPointOfApp(sf::Vector2f pointOfApp) //It serves as a setPosition-setter.
{
    if(mPointOfApp != pointOfApp)
    {
        name.move(pointOfApp - mPointOfApp);
        mPointOfApp = pointOfApp;
        name.setLinkingPoint(mPointOfApp);
        this->setAllPoints();
    }
}

void gs::Vector::setUnits(float units)
{
    units = units < 0.f ? 0.f : units;

    if(mUnits != units)
    {
        mUnits = units;

        this->setAllPoints();
    }
}

void gs::Vector::setHeightUnitsPerUnit(float heightUnitsPerUnit)
{
    heightUnitsPerUnit = heightUnitsPerUnit < 0.f ? 0.f : heightUnitsPerUnit;

    if(mHeightUnitsPerUnit != heightUnitsPerUnit)
    {
        mHeightUnitsPerUnit = heightUnitsPerUnit;

        this->setAllPoints();
    }
}

void gs::Vector::setThickness(unsigned short thickness)
{
    thickness -= thickness % 2;
    thickness = thickness < mMinThickness ? mMinThickness : thickness;

    if(mThickness != thickness)
    {
        mThickness = thickness;
        mMinVectorLength = 2 + ceil((mThickness / 2 + 2) * sqrt(3));
        mMaxBorderThickness = mThickness / 2 - 1;
        this->setAllPoints();
    }
}

void gs::Vector::setFillColor(sf::Color const innerColor)
{
    if(mInnerColor != innerColor)
    {
        mInnerColor = innerColor;
        this->setInnerPointsColor();
    }
}

void gs::Vector::updateBorderVisibility()
{
    sf::Color const &color = mBorderVisibility ? mBorderColor : mInnerColor;

    for(unsigned short i = 0; i < mAllOuterVerticesNum; ++i)
    {
        mArrow[i].color = color;
    }
}

void gs::Vector::setRotation(float degreeAngle)
{
    degreeAngle = fmod(degreeAngle, 360.f);

    if(mDeegreAngle != degreeAngle)
    {
        this->setAllPointsPosition();
        this->setDeegreAngle(degreeAngle);
        this->rotatePoints(degreeAngle);
    }
}
//setters/


void gs::Vector::move(sf::Vector2f const &displacement)
{
    mPointOfApp += displacement;
    name.setLinkingPoint(mPointOfApp);
    name.move(displacement);

    this->setAllPoints();
}

void gs::Vector::rotate(float degreeAngle)
{
    degreeAngle = fmod(degreeAngle, 360.f);

    if(fabs(degreeAngle) != mMinAngleDiff)
    {
        this->addDeegreAngle(degreeAngle);
        this->rotatePoints(degreeAngle);
    }
}


void gs::Vector::setDeegreAngle(float const degreeAngle)
{
    mDeegreAngle = fmod(degreeAngle, 360.f);
}

void gs::Vector::addDeegreAngle(float const degreeAngle)
{
    mDeegreAngle = fmod(mDeegreAngle + degreeAngle, 360.f);
}

void gs::Vector::rotatePoints(float const degreeAngle)
{
    float const &&radianAngle = fmod(-degreeAngle, 360.f) * (LD_PI / 180.0);

    for(sf::Vertex& point : mArrow)
    {
        point.position = gs::getRotatedPointAroundOrigin(point.position, mPointOfApp, radianAngle);
    }
}

void gs::Vector::setAllPoints()
{
    this->setAllPointsPosition();
    this->rotatePoints(mDeegreAngle);
}

void gs::Vector::setAllPointsPosition()
{
    this->setInnerPointsPosition();
    this->updateBorderThickness();
}

void gs::Vector::resetArrow()
{
    mArrow = std::move(
        std::vector<sf::Vertex>(  mOuterBaseVerticesNum + mOuterTipVerticesNum
                                + mInnerBaseVerticesNum + mInnerTipVerticesNum));
}


void gs::Vector::setInnerPointsPosition()
{
    unsigned short const &&baseLenHalf = mThickness / 2;
    unsigned short const &&tipWidthHalf = baseLenHalf + 2;
    float const &&tipHeight = tipWidthHalf * sqrt(3);
    float &&rectWidth = mUnits * mHeightUnitsPerUnit;
    unsigned short arrIdx = mAllOuterVerticesNum;

    rectWidth = (rectWidth < mMinVectorLength ? mMinVectorLength : rectWidth) - tipHeight;

    mArrow[arrIdx++].position = sf::Vector2f(mPointOfApp.x + mBorderThickness, mPointOfApp.y + baseLenHalf - mBorderThickness);
    mArrow[arrIdx++].position = sf::Vector2f(mPointOfApp.x + rectWidth + 1, mPointOfApp.y + baseLenHalf - mBorderThickness);
    mArrow[arrIdx++].position = sf::Vector2f(mPointOfApp.x + rectWidth + 1, mPointOfApp.y - baseLenHalf + mBorderThickness);
    mArrow[arrIdx++].position = sf::Vector2f(mPointOfApp.x + mBorderThickness, mPointOfApp.y - baseLenHalf + mBorderThickness);
    mArrow[arrIdx++].position = sf::Vector2f(mPointOfApp.x + rectWidth, mPointOfApp.y + tipWidthHalf - mBorderThickness);
    mArrow[arrIdx++].position = sf::Vector2f(mPointOfApp.x + rectWidth + tipHeight - mBorderThickness, mPointOfApp.y);
    mArrow[arrIdx++].position = sf::Vector2f(mPointOfApp.x + rectWidth, mPointOfApp.y - tipWidthHalf + mBorderThickness);
}

void gs::Vector::updateBorderThickness()
{
    float const &&baseLenHalf = mThickness / 2;
    float const &&tipWidthHalf = baseLenHalf + 2;
    float const &&tipHeight = tipWidthHalf * sqrt(3);
    float &&rectWidth = mUnits * mHeightUnitsPerUnit;
    unsigned short arrIdx = 0;

    rectWidth = (rectWidth < mMinVectorLength ? mMinVectorLength : rectWidth) - tipHeight - 1.f;

    mArrow[arrIdx++].position = sf::Vector2f(mPointOfApp.x, mPointOfApp.y + baseLenHalf);
    mArrow[arrIdx++].position = sf::Vector2f(mPointOfApp.x + rectWidth, mPointOfApp.y + baseLenHalf);
    mArrow[arrIdx++].position = sf::Vector2f(mPointOfApp.x + rectWidth, mPointOfApp.y - baseLenHalf);
    mArrow[arrIdx++].position = sf::Vector2f(mPointOfApp.x, mPointOfApp.y - baseLenHalf);
    mArrow[arrIdx++].position = sf::Vector2f(mPointOfApp.x + rectWidth, mPointOfApp.y + tipWidthHalf);
    mArrow[arrIdx++].position = sf::Vector2f(mPointOfApp.x + rectWidth + 1, mPointOfApp.y + tipWidthHalf);
    mArrow[arrIdx++].position = sf::Vector2f(mPointOfApp.x + rectWidth + 1, mPointOfApp.y - tipWidthHalf);
    mArrow[arrIdx++].position = sf::Vector2f(mPointOfApp.x + rectWidth, mPointOfApp.y - tipWidthHalf);
    mArrow[arrIdx++].position = sf::Vector2f(mPointOfApp.x + rectWidth + 1, mPointOfApp.y + tipWidthHalf);
    mArrow[arrIdx++].position = sf::Vector2f(mPointOfApp.x + rectWidth + 1 + tipHeight, mPointOfApp.y);
    mArrow[arrIdx++].position = sf::Vector2f(mPointOfApp.x + rectWidth + 1, mPointOfApp.y - tipWidthHalf);
}

void gs::Vector::setInnerPointsColor()
{
    for(unsigned short i = mAllOuterVerticesNum; i < mTotalVertices; ++i)
    {
        mArrow[i].color = mInnerColor;
    }
}

void gs::Vector::updateBorderColor()
{
    for(unsigned short i = 0; i < mAllOuterVerticesNum; ++i)
    {
        mArrow[i].color = mBorderColor;
    }
}

void gs::Vector::setBorderThickness(unsigned short borderThickness)
{
    borderThickness -= borderThickness % 2 == 0 ? 1 : 0;
    borderThickness = borderThickness > mMaxBorderThickness ? mMaxBorderThickness : borderThickness;
    if(mBorderThickness != borderThickness)
    {
        mBorderThickness = borderThickness;
        this->setAllPointsPosition();
    }
}
