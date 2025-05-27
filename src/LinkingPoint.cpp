#include "LinkingPoint.h"


gs::LinkingPoint::LinkingPoint(sf::Vector2f const &linkingPoint):
    mLinkingPoint(linkingPoint) {}

gs::LinkingPoint::~LinkingPoint() {}

sf::Vector2f gs::LinkingPoint::getLinkPoint() const
{
    return mLinkingPoint;
}

void gs::LinkingPoint::setLinkingPoint(sf::Vector2f const &linkingPoint)
{
    mLinkingPoint = linkingPoint;
}

void gs::LinkingPoint::moveLinkingPoint(sf::Vector2f const &displacement)
{
    mLinkingPoint += displacement;
}
