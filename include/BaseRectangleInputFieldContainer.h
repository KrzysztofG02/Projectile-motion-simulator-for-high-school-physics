#ifndef BASERECTANGLEINPUTFIELDCONTAINER_H
#define BASERECTANGLEINPUTFIELDCONTAINER_H

#include "RectangleTextContainer.h"

#include <gmpxx.h>

#include <SFML/Graphics/Text.hpp>


namespace gs
{

template <typename T>
class BaseRectangleInputFieldContainer: public gs::RectangleTextContainer
{
private:
    T const mMinValue, mMaxValue;

protected:
    T mCurrentValidValue;

private:
    unsigned short const mLenLimit;
    std::string const mDecimalMarks;
    std::string const mAllowedChars;
    unsigned short const mRoundingDigitsCount;
    bool mIsCurrentValueNewBuffer;
    bool mIsCurrentValueNew;
    bool mIsValueApprovedBuffer;
    bool mIsValueApproved;


protected:
    BaseRectangleInputFieldContainer(
        sf::View const * const originViewPtr = nullptr,
        std::function<void ()> const &onHoverFunc = [] () {},
        std::function<void ()> const &onClickFunc = [] () {},
        std::function<void ()> const &preHoverFunc = [] () {},
        std::function<void ()> const &preClickFunc = [] () {},
        T const &minValue = {},
        T const &maxValue = {},
        T const &currentValidValue = {},
        unsigned short const lenLimit = 10,
        unsigned short const roundingDigitsCount = 2,
        std::string const &decimalMarks = "");

    virtual ~BaseRectangleInputFieldContainer() = default;


public:
    void setInput(sf::String const &text);
    void setInput(std::string const &text);

    bool isCurrentValueNew() const;
    bool isValueApproved() const;
    T getValue() const;
    unsigned short getRoundingDigitsCount() const;


protected:
    virtual void assignInputStringByCurrentValidValue() = 0;


private:
    virtual T convertToNumericFromString(std::string const &str) const = 0;
    virtual void round(T &value) const = 0;

    void updateIndirectly() override;
    std::pair<bool, T> isNumber() const;
    void validateInput();

    void pollEventKeyPressed(sf::Event const &event);
    void pollEventMouseButtonPressed(sf::Event const &event);
    void pollEventTextEntered(sf::Event const &event);
    void alignContentPosition() override;
};

}

#include "BaseRectangleInputFieldContainer.cpp"

#endif // BASERECTANGLEINPUTFIELDCONTAINER_H
