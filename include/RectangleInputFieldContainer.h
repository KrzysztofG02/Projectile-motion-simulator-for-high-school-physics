#ifndef RECTANGLEINPUTFIELDCONTAINER_H
#define RECTANGLEINPUTFIELDCONTAINER_H

#include "BaseRectangleInputFieldContainer.h"


namespace gs
{

template <typename T>
class RectangleInputFieldContainer: public gs::BaseRectangleInputFieldContainer<T> {};


template <>
class RectangleInputFieldContainer<mpf_class> final: public BaseRectangleInputFieldContainer<mpf_class>
{
public:
    RectangleInputFieldContainer(
        sf::View const * const originViewPtr = nullptr,
        std::function<void ()> const &onHoverFunc = [] () {},
        std::function<void ()> const &onClickFunc = [] () {},
        std::function<void ()> const &preHoverFunc = [] () {},
        std::function<void ()> const &preClickFunc = [] () {},
        mpf_class const &minValue = mpf_class("0.00"),
        mpf_class const &maxValue = mpf_class("1.00"),
        mpf_class const &currentValidValue = mpf_class("0.00"),
        unsigned short const lenLimit = 10,
        unsigned short const roundingDigitsCount = 2);


private:
    mpf_class convertToNumericFromString(std::string const &str) const override;
    void round(mpf_class &value) const override;
    void assignInputStringByCurrentValidValue() override;
};


template <>
class RectangleInputFieldContainer<long> final: public BaseRectangleInputFieldContainer<long>
{
public:
    RectangleInputFieldContainer(
        sf::View const * const originViewPtr = nullptr,
        std::function<void ()> const &onHoverFunc = [] () {},
        std::function<void ()> const &onClickFunc = [] () {},
        std::function<void ()> const &preHoverFunc = [] () {},
        std::function<void ()> const &preClickFunc = [] () {},
        long const minValue = 0,
        long const maxValue = 1,
        long const currentValidValue = 0,
        unsigned short const lenLimit = 10,
        unsigned short const roundingDigitsCount = 2);


private:
    long convertToNumericFromString(std::string const &str) const override;
    void round(long &value) const override;
    void assignInputStringByCurrentValidValue() override;
};

}

#endif // RECTANGLEINPUTFIELDCONTAINER_H
