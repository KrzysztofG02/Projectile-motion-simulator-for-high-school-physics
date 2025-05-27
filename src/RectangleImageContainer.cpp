#include "RectangleImageContainer.h"

gs::RectangleImageContainer::RectangleImageContainer(sf::View const * const originViewPtr,
                                                     std::function<void ()> const &onHoverFunc,
                                                     std::function<void ()> const &onClickFunc,
                                                     std::function<void ()> const &preHoverFunc,
                                                     std::function<void ()> const &preClickFunc,
                                                     sf::Texture const * const textPtr):
    RectangleContainer<sf::Sprite>(
        originViewPtr,
        onHoverFunc,
        onClickFunc,
        preHoverFunc,
        preClickFunc)
{
    this->setInnerLineThickness(0);
    this->setPosition(sf::Vector2f(30, 12));
    this->setSize(sf::Vector2f(25.f, 25.f));
    this->setTexture(textPtr);
    this->setFillColor(sf::Color::Transparent);
}


void gs::RectangleImageContainer::setTexture(sf::Texture const * const texturePtr)
{
    if(mTextPtr == texturePtr)
    {
        return;
    }

    mTextPtr = texturePtr;

    if(mTextPtr)
    {
        mContent.setTexture(*mTextPtr);
        mContent.setScale(sf::Vector2f(this->getSize().x / mTextPtr->getSize().x,
                                       this->getSize().y / mTextPtr->getSize().y));
    }
}
