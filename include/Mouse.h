#ifndef MOUSE_H
#define MOUSE_H

#include "EventsHandler.h"
#include "StatePair.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>

#include <initializer_list>
#include <unordered_map>


namespace gs
{

class Mouse final: protected gs::EventsHandler ///Singleton
{
private:
    std::unordered_map<sf::Mouse::Button const, gs::StatePair<bool>> mPressButtonsInUse;
    std::unordered_map<sf::Mouse::Wheel const, short> mWheelsInUse;

    sf::RenderWindow const &mTargetWindow;
    sf::Vector2i mPreviousPosition;
    sf::Vector2i mCurrentPosition;


private:
    Mouse(sf::RenderWindow const &targetWindow,
          std::initializer_list<sf::Mouse::Button> const &pressButtonsInUse = {},
          std::initializer_list<sf::Mouse::Wheel> const &wheelsInUse = {});

public:
    static Mouse & getInstance(sf::RenderWindow const &targetWindow,
                               std::initializer_list<sf::Mouse::Button> const &pressButtonsInUse = {},
                               std::initializer_list<sf::Mouse::Wheel> const &wheelsInUse = {});


private:
    void updateButtons();
    void updatePosition();
    void pollEventMouseWheelScrolled(sf::Event const &event);

public:
    void update();

    ///Put the function in a place where the wheel
    ///delta is no longer needed to read in the iteration.
    void resetWheelsDelta();

    short getWheelDelta(sf::Mouse::Wheel const wheel) const;
    bool wasButtonPressed(sf::Mouse::Button const button) const;
    bool isButtonPressed(sf::Mouse::Button const button) const;

    sf::Vector2i getPreviousPosition() const;
    sf::Vector2i getCurrentPosition() const;
    sf::Vector2i getPositionDelta() const;

    sf::Vector2f getPreviousCoords() const;
    sf::Vector2f getCoordsDelta() const;
    sf::Vector2f getCurrentCoords() const;

    sf::Vector2f getPreviousCoords(sf::View const &view) const;
    sf::Vector2f getCurrentCoords(sf::View const &view) const;
    sf::Vector2f getCoordsDelta(sf::View const &view) const;
};

}

#endif // MOUSE_H
