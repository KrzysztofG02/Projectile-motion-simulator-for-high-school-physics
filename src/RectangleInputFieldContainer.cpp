#include "RectangleInputFieldContainer.h"

#include "StringFunctions.h"

using namespace std;


gs::RectangleInputFieldContainer<mpf_class>::RectangleInputFieldContainer(
    sf::View const * const originViewPtr,
    std::function<void ()> const &onHoverFunc,
    std::function<void ()> const &onClickFunc,
    std::function<void ()> const &preHoverFunc,
    std::function<void ()> const &preClickFunc,
    mpf_class const &minValue,
    mpf_class const &maxValue,
    mpf_class const &currentValidValue,
    unsigned short const lenLimit,
    unsigned short const roundingDigitsCount):
        BaseRectangleInputFieldContainer<mpf_class>(
            originViewPtr, onHoverFunc, onClickFunc, preHoverFunc, preClickFunc,
            minValue, maxValue, currentValidValue, lenLimit, roundingDigitsCount, ".")
{
    this->assignInputStringByCurrentValidValue();
}


mpf_class gs::RectangleInputFieldContainer<mpf_class>::convertToNumericFromString(std::string const &str) const
{
    return mpf_class(str);
}

void gs::RectangleInputFieldContainer<mpf_class>::round(mpf_class &value) const
{
    string const &&format = "%." + to_string(this->getRoundingDigitsCount()) + "Ff";
    char *buffer = nullptr;

    gmp_asprintf(&buffer, format.c_str(), value);

    value = mpf_class(buffer);

    free(buffer);
}

void gs::RectangleInputFieldContainer<mpf_class>::assignInputStringByCurrentValidValue()
{
    string const &&format = "%." + to_string(this->getRoundingDigitsCount()) + "Ff";
    char *buffer = nullptr;

    gmp_asprintf(&buffer, format.c_str(), mCurrentValidValue.get_mpf_t());
    mContent.setString(string(buffer));
    free(buffer);
}



gs::RectangleInputFieldContainer<long>::RectangleInputFieldContainer(
    sf::View const * const originViewPtr,
    std::function<void ()> const &onHoverFunc,
    std::function<void ()> const &onClickFunc,
    std::function<void ()> const &preHoverFunc,
    std::function<void ()> const &preClickFunc,
    long const minValue,
    long const maxValue,
    long const currentValidValue,
    unsigned short const lenLimit,
    unsigned short const roundingDigitsCount):
        BaseRectangleInputFieldContainer<long>(
            originViewPtr, onHoverFunc, onClickFunc, preHoverFunc, preClickFunc,
            minValue, maxValue, currentValidValue, lenLimit, roundingDigitsCount)
{
    this->assignInputStringByCurrentValidValue();
}


long gs::RectangleInputFieldContainer<long>::convertToNumericFromString(std::string const &str) const
{
    return stol(str);
}

void gs::RectangleInputFieldContainer<long>::round(long &value) const
{

}

void gs::RectangleInputFieldContainer<long>::assignInputStringByCurrentValidValue()
{
    mContent.setString(to_string(mCurrentValidValue));
}
