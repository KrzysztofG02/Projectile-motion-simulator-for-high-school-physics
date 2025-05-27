#ifndef TABLE_H
#define TABLE_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>

#include <cstddef>


namespace gs
{

template <std::size_t N>
class Table final: public sf::Drawable
{
private:
    sf::RectangleShape mFrame;
    std::array<sf::RectangleShape, N> mElementSpaces;
    std::array<sf::Sprite, N> mImages;
    std::array<sf::Text, N> mNames;


public:
    Table(sf::RectangleShape const &frame = {},
          std::array<sf::Sprite, N> const &images = {},
          std::array<sf::Text, N> const &names = {},
          float elementSpacesHeight = 0.f);


private:
    //from sf::Drawable
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    //from sf::Drawable/

public:
    short update(sf::View const &view) const;
};

}

#include "Table.cpp"

#endif // TABLE_H
