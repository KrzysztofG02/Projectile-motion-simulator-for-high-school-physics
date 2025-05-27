#include "App.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

using namespace std;


template <typename T>
gs::RectangleContainer<T>::RectangleContainer(sf::View const * const originViewPtr,
                                              std::function<void ()> const &onHoverFunc,
                                              std::function<void ()> const &onClickFunc,
                                              std::function<void ()> const &preHoverFunc,
                                              std::function<void ()> const &preClickFunc):
    mRectangle(),
    mOnHoverFunc(onHoverFunc),
    mOnClickFunc(onClickFunc),
    mPreHoverFunc(preHoverFunc),
    mPreClickFunc(preClickFunc),
    mIsActive(false),
    mIsEnabled(true),

    mOriginViewPtr(originViewPtr),
    mContent()
{
    this->addEvent(sf::Event::MouseButtonPressed, bind(&RectangleContainer<T>::pollEventMouseButtonPressed, ref(*this), placeholders::_1));
    this->setInnerLineThickness(3);
}


template <typename T>
void gs::RectangleContainer<T>::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mRectangle, states);
    target.draw(mContent, states);
}


template <typename T>
void gs::RectangleContainer<T>::activate()
{
    mIsActive = true;
}

template <typename T>
void gs::RectangleContainer<T>::deactivate()
{
    mIsActive = false;
}


template <typename T>
bool gs::RectangleContainer<T>::isEnabled() const
{
    return mIsEnabled;
}

template <typename T>
sf::Vector2f gs::RectangleContainer<T>::getPosition() const
{
    return mRectangle.getPosition();
}

template <typename T>
sf::Vector2f gs::RectangleContainer<T>::getInnerPosition() const
{
    return   mRectangle.getPosition()
           + sf::Vector2f(this->getInnerLineThickness(), this->getInnerLineThickness());
}

template <typename T>
sf::Vector2f gs::RectangleContainer<T>::getSize() const
{
    return mRectangle.getSize();
}

template <typename T>
sf::Vector2f gs::RectangleContainer<T>::getInnerSize() const
{
    return   mRectangle.getSize()
           - 2.f * sf::Vector2f(this->getInnerLineThickness(), this->getInnerLineThickness());
}

template <typename T>
sf::Color gs::RectangleContainer<T>::getFillColor() const
{
    return mRectangle.getFillColor();
}

template <typename T>
sf::Color gs::RectangleContainer<T>::getInnerLineColor() const
{
    return mRectangle.getOutlineColor();
}

template <typename T>
unsigned short gs::RectangleContainer<T>::getInnerLineThickness() const
{
    return static_cast<unsigned short>(-mRectangle.getOutlineThickness());
}

template <typename T>
std::function<void ()> const & gs::RectangleContainer<T>::getOnHoverFunc() const
{
    return mOnHoverFunc;
}

template <typename T>
std::function<void ()> const & gs::RectangleContainer<T>::getOnClickFunc() const
{
    return mOnClickFunc;
}

template <typename T>
std::function<void ()> const & gs::RectangleContainer<T>::getPreHoverFunc() const
{
    return mPreHoverFunc;
}

template <typename T>
std::function<void ()> const & gs::RectangleContainer<T>::getPreClickFunc() const
{
    return mPreClickFunc;
}

template <typename T>
bool gs::RectangleContainer<T>::isActive() const
{
    return mIsActive;
}

template <typename T>
bool gs::RectangleContainer<T>::contains(sf::Vector2f const &point) const
{
    return mRectangle.getGlobalBounds().contains(point);
}


template <typename T>
void gs::RectangleContainer<T>::setEnabled(bool const isEnabled)
{
    if(mIsEnabled != isEnabled)
    {
        mIsEnabled = isEnabled;

        if(isEnabled)
        {
            this->setFillColor(sf::Color::White);
        }
        else
        {
            this->setFillColor(sf::Color::Red);
        }
    }
}

template <typename T>
void gs::RectangleContainer<T>::setPosition(sf::Vector2f const &position)
{
    mRectangle.setPosition(position);

    this->alignContentPosition();
}

template <typename T>
void gs::RectangleContainer<T>::setSize(sf::Vector2f const &size)
{
    mRectangle.setSize(size);
}

template <typename T>
void gs::RectangleContainer<T>::setFillColor(sf::Color const fillColor)
{
    mRectangle.setFillColor(fillColor);
}

template <typename T>
void gs::RectangleContainer<T>::setContentFillColor(sf::Color const fillColor)
{
    mContent.setFillColor(fillColor);
}

template <typename T>
void gs::RectangleContainer<T>::setInnerLineColor(sf::Color const innerLineColor)
{
    mRectangle.setOutlineColor(innerLineColor);
}

template <typename T>
void gs::RectangleContainer<T>::setInnerLineThickness(unsigned short const innerlineThickness)
{
    mRectangle.setOutlineThickness(-static_cast<float>(innerlineThickness));
}

template <typename T>
void gs::RectangleContainer<T>::setOnHoverFunc(std::function<void ()> const &onHoverFunc)
{
    mOnHoverFunc = onHoverFunc;
}

template <typename T>
void gs::RectangleContainer<T>::setOnClickFunc(std::function<void ()> const &onClickFunc)
{
    mOnClickFunc = onClickFunc;
}

template <typename T>
void gs::RectangleContainer<T>::setPreHoverFunc(std::function<void ()> const &preHoverFunc)
{
    mPreHoverFunc = preHoverFunc;
}

template <typename T>
void gs::RectangleContainer<T>::setPreClickFunc(std::function<void ()> const &preClickFunc)
{
    mPreClickFunc = preClickFunc;
}


template <typename T>
void gs::RectangleContainer<T>::pollEventMouseButtonPressed(sf::Event const &event)
{
    if(event.mouseButton.button != gs::App::mainButton)
    {
        return;
    }

    if(mRectangle.getGlobalBounds().contains(gs::App::mouse.getCurrentCoords(*mOriginViewPtr)))
    {
        this->activate();

        if(this->isEnabled())
        {
            this->onClick();
        }
    }
    else
    {
        this->deactivate();
    }
}


template <typename T>
void gs::RectangleContainer<T>::update()
{
    if(this->contains(gs::App::mouse.getCurrentCoords(*mOriginViewPtr)))
    {
        this->onHover();
    }
    else
    {
        this->restoreToPreHoverState();
    }

    this->updateIndirectly();
}

template <typename T>
void gs::RectangleContainer<T>::updateIndirectly()
{

}


template <typename T>
void gs::RectangleContainer<T>::onHover()
{
     mOnHoverFunc();
}

template <typename T>
void gs::RectangleContainer<T>::onClick()
{
    mOnClickFunc();
}

template <typename T>
void gs::RectangleContainer<T>::restoreToPreHoverState()
{
    mPreHoverFunc();
}

template <typename T>
void gs::RectangleContainer<T>::restoreToPreClickState()
{
    mPreClickFunc();
}


template <typename T>
void gs::RectangleContainer<T>::alignContentPosition()
{
    sf::FloatRect const bounds = mContent.getLocalBounds();

    mContent.setOrigin(bounds.getPosition() + bounds.getSize() / 2.f);
    mContent.setPosition(this->getPosition() + this->getSize() / 2.f);
}
