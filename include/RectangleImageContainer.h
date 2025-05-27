#ifndef RECTANGLEIMAGECONTAINER_H
#define RECTANGLEIMAGECONTAINER_H

#include "RectangleContainer.h"
#include "BinarySwitch.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>


namespace gs
{

class RectangleImageContainer: public RectangleContainer<sf::Sprite>
{
private:
    sf::Texture const *mTextPtr;


public:
    RectangleImageContainer(sf::View const * const originViewPtr = nullptr,
                            std::function<void ()> const &onHoverFunc = [] () {},
                            std::function<void ()> const &onClickFunc = [] () {},
                            std::function<void ()> const &preHoverFunc = [] () {},
                            std::function<void ()> const &preClickFunc = [] () {},
                            sf::Texture const * const textPtr = nullptr);


public:
    void setTexture(sf::Texture const * const texturePtr);
};

}

#endif // RECTANGLEIMAGECONTAINER_H
