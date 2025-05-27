#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <cstring>
#include <iostream>

#include "Border.h"
#include "BoundingRect.h"
#include "Math.h"
#include "Mouse.h"
#include "TextName.h"


///vertices order in the class
/*
                        7-6
                        |  \
                        |   \
9-----------------------8    \
|                             \
0                              5
|                             /
1-----------------------2    /
                        |   /
                        |  /
                        3-4
*/
///vertices order in the class/

namespace gs
{

class Vector final: public sf::Drawable, public gs::Border
{
private:
    static constexpr float mMinAngleDiff = 0.5f;
    static constexpr float mMinUnitsToRender = FLT_EPSILON;
    static constexpr float mTitleScale = 0.25f;

    static constexpr unsigned short mMinThickness = 4;

    static constexpr unsigned short mOuterBaseVerticesNum = 8;
    static constexpr unsigned short mOuterTipVerticesNum  = 3;
    static constexpr unsigned short mAllOuterVerticesNum  = mOuterBaseVerticesNum + mOuterTipVerticesNum;

    static constexpr unsigned short mInnerBaseVerticesNum = 4;
    static constexpr unsigned short mInnerTipVerticesNum  = 3;
    static constexpr unsigned short mAllInnerVerticesNum  = mInnerBaseVerticesNum + mInnerTipVerticesNum;

    static constexpr unsigned short mTotalVertices = mAllOuterVerticesNum + mAllInnerVerticesNum;


private:
    sf::Vector2f mPointOfApp;
    std::vector<sf::Vertex> mArrow;

    float mUnits;
    float mHeightUnitsPerUnit;
    float mDeegreAngle;

    unsigned short mThickness;

    sf::Color mInnerColor;

    unsigned short mMinVectorLength;
    unsigned short mMaxBorderThickness;

public:
    gs::TextName name;


public:
    Vector(sf::Vector2f const &pointOfApp = sf::Vector2f(),
           float const units = 10.f,
           float const heightUnitsPerUnit = 3.f,
           float const deegreAngle = 0.f,
           unsigned short const thickness = mMinThickness,
           sf::Color const innerColor = sf::Color::Magenta,
           unsigned short const borderThickness = 1,
           bool const borderVisibility = true,
           sf::Color const borderColor = sf::Color::Green);
    Vector(Vector const &) = default;
    Vector & operator=(Vector const &) = default;
    Vector(Vector&&) = default;
    Vector & operator=(Vector &&) = default;
    ~Vector() = default;


private:
    //from sf::Drawable
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    //from sf::Drawable/

public:
    //from gs::Border
    void setBorderThickness(unsigned short borderThickness); //odd positive int only //override, non-virtual
    //from gs::Border/

    //getters
    sf::Vector2f getPointOfApp() const;
    float getUnits() const;
    float getHeightUnitsPerUnit() const;
    float getDeegreAngle() const;
    sf::Color getInnerColor() const;
    unsigned short getMaxBorderThickness() const;
    //getters/

    //setters
    void setPointOfApp(sf::Vector2f pointOfApp); //It serves as a setPosition-setter.
    void setUnits(float units);
    void setHeightUnitsPerUnit(float heightUnitsPerUnit);
    void setThickness(unsigned short thickness); //even positive integer only
    void setFillColor(sf::Color const innerColor);
    void setRotation(float degreeAngle);
    //setters/

    void move(sf::Vector2f const &displacement);
    void rotate(float degreeAngle);


private:
    //from gs::Border
    virtual void updateBorderVisibility() override;
    virtual void updateBorderThickness() override;
    virtual void updateBorderColor() override;
    //from gs::Border/

    const bool canBuildVector() const;

    void setDeegreAngle(float const degreeAngle);
    void addDeegreAngle(float const degreeAngle);
    void rotatePoints(float const degreeAngle);

    void setAllPoints();
    void setAllPointsPosition();

    void resetArrow();

    void setInnerPointsPosition();
    void setInnerPointsColor();
};

}


#endif // VECTOR_H*

