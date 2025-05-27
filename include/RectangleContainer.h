#ifndef RECTANGLECONTAINER_H
#define RECTANGLECONTAINER_H

#include "EventsHandler.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/View.hpp>

#include <functional>


namespace gs
{

template <typename T>
class RectangleContainer: public sf::Drawable, public gs::EventsHandler
{
private:
    sf::RectangleShape mRectangle;

    std::function<void ()> mOnHoverFunc, mOnClickFunc, mPreHoverFunc, mPreClickFunc;

    bool mIsActive;
    bool mIsEnabled;

protected:
    sf::View const *mOriginViewPtr;
    T mContent;


public:
    RectangleContainer(sf::View const * const originViewPtr = nullptr,
                       std::function<void ()> const &onHoverFunc = [] () {},
                       std::function<void ()> const &onClickFunc = [] () {},
                       std::function<void ()> const &preHoverFunc = [] () {},
                       std::function<void ()> const &preClickFunc = [] () {});

public:
    virtual ~RectangleContainer() = default;


private:
    //from gs::Drawable
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    //from gs::Drawable/

    void activate();
    void deactivate();


    //getters
public:
    bool isEnabled() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getInnerPosition() const;
    sf::Vector2f getSize() const;
    sf::Vector2f getInnerSize() const;
    sf::Color getFillColor() const;
    sf::Color getInnerLineColor() const;
    unsigned short getInnerLineThickness() const;
    std::function<void ()> const & getOnHoverFunc() const;
    std::function<void ()> const & getOnClickFunc() const;
    std::function<void ()> const & getPreHoverFunc() const;
    std::function<void ()> const & getPreClickFunc() const;
protected:
    bool isActive() const;
    bool contains(sf::Vector2f const &point) const;
    //getters/

    //setters
public:
    void setEnabled(bool const isEnabled);
    void setPosition(sf::Vector2f const &position);
    virtual void setSize(sf::Vector2f const &size);
    void setFillColor(sf::Color const fillColor);
    void setContentFillColor(sf::Color const fillColor);
    void setInnerLineColor(sf::Color const innerLineColor);
    void setInnerLineThickness(unsigned short const innerlineThickness);
    void setOnHoverFunc(std::function<void ()> const &onHoverFunc);
    void setOnClickFunc(std::function<void ()> const &onClickFunc);
    void setPreHoverFunc(std::function<void ()> const &preHoverFunc);
    void setPreClickFunc(std::function<void ()> const &preClickFunc);
    //setters/

public:
    void pollEventMouseButtonPressed(sf::Event const &event);

    void update();


private:
    virtual void updateIndirectly();
    void onHover();
    void onClick();
    void restoreToPreHoverState();
    void restoreToPreClickState();


protected:
    virtual void alignContentPosition();
};

}

#include "RectangleContainer.cpp"

#endif // RECTANGLECONTAINER_H
