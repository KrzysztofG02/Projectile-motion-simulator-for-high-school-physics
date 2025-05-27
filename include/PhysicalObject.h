#ifndef PHYSICALOBJECT_H
#define PHYSICALOBJECT_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


namespace gs
{

template <typename TimeType>
class PhysicalObject: public sf::Drawable
{
protected:
    bool mEquilibrium;


protected:
    PhysicalObject(bool const &equilibrium = false);


public:
    virtual ~PhysicalObject() = default;


private:
    //from sf::Drawable
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override = 0;
    //from sf::Drawable/

    virtual void affectObject(TimeType const &timeD = {}) = 0;


public:
    void update(TimeType const &timeD);

    //getters
    bool hasEquilibrium() const;
    //getters/
};

}

#include "PhysicalObject.cpp"


#endif // PHYSICALOBJECT_H
