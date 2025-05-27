#ifndef LINKINGPOINT_H
#define LINKINGPOINT_H

#include <SFML/System/Vector2.hpp>


namespace gs
{

class LinkingPoint
{
protected:
    sf::Vector2f mLinkingPoint;


public:
    LinkingPoint(sf::Vector2f const &linkingPoint = sf::Vector2f());
    virtual ~LinkingPoint();


public:
    sf::Vector2f getLinkPoint() const;
    void setLinkingPoint(sf::Vector2f const &linkingPoint);
    void moveLinkingPoint(sf::Vector2f const &displacement);
};

}

#endif // LINKINGPOINT_H
