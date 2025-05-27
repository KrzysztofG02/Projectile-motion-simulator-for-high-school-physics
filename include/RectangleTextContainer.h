#ifndef RECTANGLETEXTCONTAINER_H
#define RECTANGLETEXTCONTAINER_H

#include "RectangleContainer.h"

#include <SFML/Graphics/Text.hpp>


namespace gs
{

class RectangleTextContainer: public RectangleContainer<sf::Text>
{
private:
    float mTextToContainerHeightRatio;


public:
    RectangleTextContainer(sf::View const * const originViewPtr = nullptr,
                           std::function<void ()> const &onHoverFunc = [] () {},
                           std::function<void ()> const &onClickFunc = [] () {},
                           std::function<void ()> const &preHoverFunc = [] () {},
                           std::function<void ()> const &preClickFunc = [] () {},
                           sf::String const &text = {});


public:
    sf::String getString() const;

    //from gs::RectangleContainer
    void setSize(sf::Vector2f const &size) override;
    //from gs::RectangleContainer/

    void setFont(sf::Font const &font);
    void setString(sf::String const &text);
    void setString(std::string const &text);
    void setCharacterSize(size_t const characterSize);
};

}

#endif // RECTANGLETEXTCONTAINER_H
