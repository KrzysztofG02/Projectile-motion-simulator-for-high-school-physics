#ifndef BORDER_H
#define BORDER_H

#include <SFML/Graphics/Color.hpp>


namespace gs
{

class Border //abstract
{
protected:
    bool mBorderVisibility;
    unsigned short mBorderThickness;
    sf::Color mBorderColor;


protected:
    Border(bool const borderVisibility = true,
           unsigned short const borderThickness = 1,
           sf::Color const borderColor = sf::Color::Green);
    virtual ~Border();


public:
    //getters
    virtual bool getBorderVisibility() const;
    virtual unsigned short getBorderThickness() const;
    virtual sf::Color getBorderColor() const;
    //getters/

    //setters
    void setBorderVisibility(bool const borderVisibility);
    void setBorderThickness(unsigned short const borderThickness);
    void setBorderColor(sf::Color const borderColor);
    //setters/


protected:
    virtual void updateBorderVisibility() = 0;
    virtual void updateBorderThickness() = 0;
    virtual void updateBorderColor() = 0;
};

}

#endif // BORDER_H
