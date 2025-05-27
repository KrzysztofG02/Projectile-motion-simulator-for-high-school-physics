#include "TextName.h"

#include <SFML/Graphics/RenderTarget.hpp>


using namespace std;


gs::TextName::TextName(sf::Vector2f const &linkingPoint,
                       sf::String const &string,
                       sf::Font const &font,
                       unsigned const characterSize):
    gs::LinkingPoint(linkingPoint),
    mText(string, font, characterSize),
    mRelocationFlag(false)
{
    mText.setOrigin(mText.getLocalBounds().width / 2.f, mText.getLocalBounds().height / 2.f);
}

gs::TextName::~TextName() {}


void gs::TextName::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if(mRelocationFlag)
    {
        sf::VertexArray line = sf::VertexArray(sf::Lines, 2);
        line[0] = sf::Vertex(mLinkingPoint, sf::Color::Red);
        line[1] = sf::Vertex(mText.getPosition(), sf::Color::Green);

        target.draw(line, states);

        mRelocationFlag = false;
    }

    target.draw(mText, states);
}


sf::Vector2f gs::TextName::getPosition() const
{
    return mText.getPosition();
}

sf::String gs::TextName::getString() const
{
    return mText.getString();
}

sf::Vector2f gs::TextName::getScale() const
{
    return mText.getScale();
}

sf::Font const & gs::TextName::getFont() const
{
    return *mText.getFont();
}

unsigned gs::TextName::getCharacterSize() const
{
    return mText.getCharacterSize();
}

sf::Color gs::TextName::getFillColor() const
{
    return mText.getFillColor();
}

sf::Color gs::TextName::getOutlineColor() const
{
    return mText.getOutlineColor();
}

float gs::TextName::getOutlineThickness() const
{
    return mText.getOutlineThickness();
}


void gs::TextName::setPosition(const sf::Vector2f &position)
{
    mText.setPosition(position);
}

void gs::TextName::setString(sf::String const &string)
{
    mText.setString(string);
    mText.setOrigin(mText.getLocalBounds().width / 2.f, mText.getLocalBounds().height / 2.f);
}

void gs::TextName::setScale(sf::Vector2f const &scale)
{
    mText.setScale(scale);
}

void gs::TextName::setFont(sf::Font const &font)
{
    mText.setFont(font);
    mText.setOrigin(mText.getLocalBounds().width / 2.f, mText.getLocalBounds().height / 2.f);
}

void gs::TextName::setCharacterSize(unsigned const size)
{
    mText.setCharacterSize(size);
    mText.setOrigin(mText.getLocalBounds().width / 2.f, mText.getLocalBounds().height / 2.f);
}

void gs::TextName::setFillColor(sf::Color const color)
{
    mText.setFillColor(color);
}

void gs::TextName::setOutlineColor(sf::Color const color)
{
    mText.setOutlineColor(color);
}

void gs::TextName::setOutlineThickness(float const thickness)
{
    mText.setOutlineThickness(thickness);
    mText.setOrigin(mText.getLocalBounds().width / 2.f, mText.getLocalBounds().height / 2.f);
}


void gs::TextName::move(const sf::Vector2f &displacement)
{
    mText.move(displacement);
}
