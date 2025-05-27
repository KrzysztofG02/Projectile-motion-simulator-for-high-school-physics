#include "Mouse.h"

using namespace std;


gs::Mouse::Mouse(sf::RenderWindow const &targetWindow,
                 std::initializer_list<sf::Mouse::Button> const &pressButtonsInUse,
                 std::initializer_list<sf::Mouse::Wheel> const &wheelsInUse):
    mPressButtonsInUse{},
    mWheelsInUse{},
    mTargetWindow(targetWindow),
    mPreviousPosition(sf::Mouse::getPosition(mTargetWindow)),
    mCurrentPosition(mPreviousPosition)
{
    for(sf::Mouse::Button const button : pressButtonsInUse)
    {
        mPressButtonsInUse[button] = gs::StatePair<bool>{false, false};
    }

    for(sf::Mouse::Wheel const &wheel : wheelsInUse)
    {
        mWheelsInUse[wheel] = 0;
    }

    if(mWheelsInUse.size() > 0)
    {
        this->addEvent(sf::Event::MouseWheelScrolled, bind(&Mouse::pollEventMouseWheelScrolled, ref(*this), placeholders::_1));
    }
}

gs::Mouse & gs::Mouse::getInstance(sf::RenderWindow const &targetWindow,
                                   std::initializer_list<sf::Mouse::Button> const &pressButtonsInUse,
                                   std::initializer_list<sf::Mouse::Wheel> const &wheelsInUse)
{
    static Mouse instance = Mouse(targetWindow, pressButtonsInUse, wheelsInUse);

    return instance;
}


void gs::Mouse::updateButtons()
{
    for(pair<sf::Mouse::Button const, gs::StatePair<bool>> &button : mPressButtonsInUse)
    {
        button.second.prev = button.second.curr;
        button.second.curr = sf::Mouse::isButtonPressed(button.first);
    }
}

void gs::Mouse::updatePosition()
{
    mPreviousPosition = mCurrentPosition;
    mCurrentPosition = sf::Mouse::getPosition(mTargetWindow);
}


void gs::Mouse::update()
{
    this->updateButtons();
    this->updatePosition();
}


void gs::Mouse::pollEventMouseWheelScrolled(sf::Event const &event)
{
    decltype(mWheelsInUse)::iterator const wheelInUse_it = mWheelsInUse.find(event.mouseWheelScroll.wheel);

    if(wheelInUse_it != mWheelsInUse.end())
    {
        wheelInUse_it->second = event.mouseWheelScroll.delta;
    }
}

void gs::Mouse::resetWheelsDelta()
{
    for(pair<sf::Mouse::Wheel const, short> &wheelInUse : mWheelsInUse)
    {
        wheelInUse.second = 0;
    }
}

short gs::Mouse::getWheelDelta(sf::Mouse::Wheel const wheel) const
{
    decltype(mWheelsInUse)::const_iterator const it = mWheelsInUse.find(wheel);

    if(it != mWheelsInUse.end())
    {
        return it->second;
    }

    return 0;
}

bool gs::Mouse::wasButtonPressed(sf::Mouse::Button const button) const
{
    decltype(mPressButtonsInUse)::const_iterator const it = mPressButtonsInUse.find(button);

    if(it != mPressButtonsInUse.end())
    {
        return it->second.prev;
    }

    return false;
}

bool gs::Mouse::isButtonPressed(sf::Mouse::Button const button) const
{
    decltype(mPressButtonsInUse)::const_iterator const &&it = mPressButtonsInUse.find(button);

    if(it != mPressButtonsInUse.end())
    {
        return it->second.curr;
    }

    return false;
}


sf::Vector2i gs::Mouse::getPreviousPosition() const
{
    return mPreviousPosition;
}

sf::Vector2i gs::Mouse::getCurrentPosition() const
{
    return mCurrentPosition;
}

sf::Vector2i gs::Mouse::getPositionDelta() const
{
    return mCurrentPosition - mPreviousPosition;
}


sf::Vector2f gs::Mouse::getPreviousCoords() const
{
    return mTargetWindow.mapPixelToCoords(mPreviousPosition, mTargetWindow.getView());
}

sf::Vector2f gs::Mouse::getPreviousCoords(sf::View const &view) const
{
    return mTargetWindow.mapPixelToCoords(mPreviousPosition, view);
}

sf::Vector2f gs::Mouse::getCurrentCoords() const
{
    return mTargetWindow.mapPixelToCoords(mCurrentPosition, mTargetWindow.getView());
}

sf::Vector2f gs::Mouse::getCurrentCoords(sf::View const &view) const
{
    return mTargetWindow.mapPixelToCoords(mCurrentPosition, view);
}

sf::Vector2f gs::Mouse::getCoordsDelta() const
{
    return   mTargetWindow.mapPixelToCoords(mCurrentPosition, mTargetWindow.getView())
           - mTargetWindow.mapPixelToCoords(mPreviousPosition, mTargetWindow.getView());
}

sf::Vector2f gs::Mouse::getCoordsDelta(sf::View const &view) const
{
    return   mTargetWindow.mapPixelToCoords(mCurrentPosition, view)
           - mTargetWindow.mapPixelToCoords(mPreviousPosition, view);
}
