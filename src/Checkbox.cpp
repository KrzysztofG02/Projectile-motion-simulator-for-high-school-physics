#include "Checkbox.h"


gs::Checkbox::Checkbox(sf::Texture const &checkboxUncheckedTexture,
                       sf::Texture const &checkboxCheckedTexture,
                       sf::View const * const originViewPtr,
                       bool const &checkedByDefault,
                       std::function<void ()> const &onHoverFunc,
                       std::function<void ()> const &onClickFunc,
                       std::function<void ()> const &preHoverFunc,
                       std::function<void ()> const &preClickFunc):
    RectangleImageContainer(
        originViewPtr,
        onHoverFunc,
        [&, onClickFunc] () {onClickFunc(); mBinarySwitch();},
        preHoverFunc,
        preClickFunc,
        &checkboxUncheckedTexture),
    mCheckboxUncheckedTexture(checkboxUncheckedTexture),
    mCheckboxCheckedTexture(checkboxCheckedTexture),
    mBinarySwitch(
        bind(&Checkbox::setTexture, ref(*this), &mCheckboxUncheckedTexture),
        bind(&Checkbox::setTexture, ref(*this), &mCheckboxCheckedTexture))
{
    if(checkedByDefault)
    {
        mBinarySwitch();
    }
}
