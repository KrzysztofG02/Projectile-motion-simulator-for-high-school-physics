#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "RectangleImageContainer.h"


namespace gs
{

class Checkbox final: public gs::RectangleImageContainer
{
private:
    sf::Texture const &mCheckboxUncheckedTexture, &mCheckboxCheckedTexture;
    BinarySwitch mBinarySwitch;

public:
    Checkbox(sf::Texture const &checkboxUncheckedTexture,
             sf::Texture const &checkboxCheckedTexture,
             sf::View const * const originViewPtr,
             bool const &checkedByDefault = false,
             std::function<void ()> const &onHoverFunc = [] () {},
             std::function<void ()> const &onClickFunc = [] () {},
             std::function<void ()> const &preHoverFunc = [] () {},
             std::function<void ()> const &preClickFunc = [] () {});
};

}

#endif // CHECKBOX_H
