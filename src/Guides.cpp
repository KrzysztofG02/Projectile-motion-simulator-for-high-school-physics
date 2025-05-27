#include "Guides.h"


gs::Guides::Guides(sf::Vector3f const &hLine, sf::Vector3f const &vLine, sf::Color const color):
    mLines(sf::PrimitiveType::Lines, 4)
{
    this->setHLineCoords(hLine);
    this->setVLineCoords(vLine);
    this->setColor(color);
}

gs::Guides::~Guides() {}


void gs::Guides::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mLines, states);
}


sf::Vector2f gs::Guides::getPosition() const
{
    return sf::Vector2f(mLines[2].position.x, mLines[0].position.y);
}

sf::Vector3f gs::Guides::getHLineCoords() const
{
    return sf::Vector3f(mLines[0].position.x, mLines[1].position.x, mLines[1].position.y);
}

sf::Vector3f gs::Guides::getVLineCoords() const
{
    return sf::Vector3f(mLines[2].position.x, mLines[2].position.x, mLines[3].position.y);
}

sf::Color gs::Guides::getColor() const
{
    return mLines[0].color;
}


void gs::Guides::setPosition(sf::Vector2f const &position)
{
    mLines[0].position.y = position.y;
    mLines[1].position.y = position.y;
    mLines[2].position.x = position.x;
    mLines[3].position.x = position.x;
}

void gs::Guides::setHLineCoords(sf::Vector3f const &hLine)
{
    mLines[0].position.x = hLine.x;
    mLines[0].position.y = hLine.z;

    mLines[1].position.x = hLine.y;
    mLines[1].position.y = hLine.z;
}

void gs::Guides::setHLineCoords(float const x, float const y, float const z)
{
    this->setHLineCoords(sf::Vector3f(x, y, z));
}

void gs::Guides::setVLineCoords(sf::Vector3f const &vLine)
{
    mLines[2].position.x = vLine.x;
    mLines[2].position.y = vLine.y;

    mLines[3].position.x = vLine.x;
    mLines[3].position.y = vLine.z;
}

void gs::Guides::setVLineCoords(float const x, float const y, float const z)
{
    this->setVLineCoords(sf::Vector3f(x, y, z));
}

void gs::Guides::setColor(sf::Color const color)
{
    std::for_each(&mLines[0], &mLines[0] + mLines.getVertexCount(), [&color] (sf::Vertex &vertex) {vertex.color = color;});
}
