#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector3.hpp>


namespace gs
{

class Guides final: public sf::Drawable
{
private:
    sf::VertexArray mLines;
//hLine: Xhleft, Xhright, Yh
//vLine: Xv, Yvtop, Yvbottom

public:
    Guides(sf::Vector3f const &hLine = sf::Vector3f(0, 0, 0),
           sf::Vector3f const &vLine = sf::Vector3f(0, 0, 0),
           sf::Color const color = sf::Color::Black);

    virtual ~Guides();

private:
    //from sf::Drawable
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    //from sf::Drawable/

public:
    sf::Vector2f getPosition() const;
    sf::Vector3f getHLineCoords() const;
    sf::Vector3f getVLineCoords() const;
    sf::Color getColor() const;

    void setPosition(sf::Vector2f const &position);
    void setHLineCoords(sf::Vector3f const &hLine);
    void setHLineCoords(float const x, float const y, float const z);
    void setVLineCoords(sf::Vector3f const &vLine);
    void setVLineCoords(float const x, float const y, float const z);
    void setColor(sf::Color const color);
};

}

#endif // CROSSHAIR_H
