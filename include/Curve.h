#ifndef CURVE_H
#define CURVE_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>


namespace gs
{

template <std::size_t N>
class Curve final: public sf::Drawable
{
private:
    std::array<sf::Vertex, N> mPoints;


public:
    Curve(sf::Color const color = sf::Color::Black);
    ~Curve();

private:
    //from sf::Drawable
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    //from sf::Drawable/

public:
    size_t getPointCount() const;
    sf::Vertex const & operator[](size_t index) const;
    sf::Vertex & operator[](size_t index);

    void setAllPointsColor(sf::Color const color);
};

}

#include "Curve.cpp"

#endif // CURVE_H
