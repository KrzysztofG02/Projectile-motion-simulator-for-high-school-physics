#ifndef TEXTNAME_H
#define TEXTNAME_H

#include "LinkingPoint.h"

#include <SFML/Graphics/Text.hpp>


namespace gs
{

class TextName final: public sf::Drawable, public gs::LinkingPoint
{
private:
    sf::Text mText;

    mutable bool mRelocationFlag;


public:
    TextName(sf::Vector2f const &linkingPoint = sf::Vector2f(),
             sf::String const &string = "",
             sf::Font const &font = sf::Font(),
             unsigned const characterSize = 60);
    ~TextName();


private:
    //from sf::Drawable
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    //from sf::Drawable/

public:
    //getters
    sf::Vector2f getPosition() const;
    sf::String getString() const;
    sf::Vector2f getScale() const;
    sf::Font const & getFont() const;
    unsigned getCharacterSize() const;
    sf::Color getFillColor() const;
    sf::Color getOutlineColor() const;
    float getOutlineThickness() const;
    //getters/

    //setters
    void setPosition(sf::Vector2f const &position);
    void setString(sf::String const &string);
    void setScale(sf::Vector2f const &scale);
    void setFont(sf::Font const &font);
    void setCharacterSize(unsigned const size);
    void setFillColor(sf::Color const color);
    void setOutlineColor(sf::Color const color);
    void setOutlineThickness(float const thickness);
    //setters/

    void move(sf::Vector2f const &displacement);
};

}

#endif // TEXTNAME_H
