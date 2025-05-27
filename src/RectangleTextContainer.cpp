#include "RectangleTextContainer.h"

#include "App.h"


gs::RectangleTextContainer::RectangleTextContainer(sf::View const * const originViewPtr,
                                                   std::function<void ()> const &onHoverFunc,
                                                   std::function<void ()> const &onClickFunc,
                                                   std::function<void ()> const &preHoverFunc,
                                                   std::function<void ()> const &preClickFunc,
                                                   sf::String const &text):
    RectangleContainer<sf::Text>(originViewPtr, onHoverFunc, onClickFunc, preHoverFunc, preClickFunc)
{
    this->setFont(gs::App::arialFont);
    this->setString(text);
    this->setContentFillColor(sf::Color::Black);

    this->alignContentPosition();
}


sf::String gs::RectangleTextContainer::getString() const
{
    return mContent.getString();
}


void gs::RectangleTextContainer::setSize(sf::Vector2f const &size)
{
    RectangleContainer::setSize(size);
}


void gs::RectangleTextContainer::setFont(sf::Font const &font)
{
    mContent.setFont(font);
}

void gs::RectangleTextContainer::setString(sf::String const &text)
{
    mContent.setString(text);
}

void gs::RectangleTextContainer::setString(std::string const &text)
{
    mContent.setString(text);
}

void gs::RectangleTextContainer::setCharacterSize(size_t const characterSize)
{
    mContent.setCharacterSize(characterSize);

    this->alignContentPosition();
}
