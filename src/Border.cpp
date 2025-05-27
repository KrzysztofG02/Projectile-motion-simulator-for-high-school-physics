#include "Border.h"


gs::Border::Border(bool const borderVisibility,
                   unsigned short const borderThickness,
                   sf::Color const borderColor):
                   mBorderVisibility(borderVisibility),
                   mBorderThickness(borderThickness),
                   mBorderColor(borderColor) {}

gs::Border::~Border() {}


bool gs::Border::getBorderVisibility() const
{
    return mBorderVisibility;
}

unsigned short gs::Border::getBorderThickness() const
{
    return mBorderThickness;
}

sf::Color gs::Border::getBorderColor() const
{
    return mBorderColor;
}


void gs::Border::setBorderVisibility(bool const borderVisibility)
{
    if(mBorderVisibility != borderVisibility)
    {
        mBorderVisibility = borderVisibility;
        this->updateBorderVisibility();
    }
}

void gs::Border::setBorderThickness(unsigned short const borderThickness)
{
    if(mBorderThickness != borderThickness)
    {
        mBorderThickness = borderThickness;
        this->updateBorderThickness();
    }
}

void gs::Border::setBorderColor(sf::Color const borderColor)
{
    if(mBorderColor != borderColor)
    {
        mBorderColor = borderColor;
        this->updateBorderColor();
    }
}
