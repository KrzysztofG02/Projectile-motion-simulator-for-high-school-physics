#include "App.h"
#include <iostream>
using namespace std;
template <std::size_t N>
gs::Table<N>::Table(sf::RectangleShape const &frame,
                    std::array<sf::Sprite, N> const &images,
                    std::array<sf::Text, N> const &names,
                    float elementSpacesHeight):
    mFrame(frame),
    mElementSpaces(),
    mImages(images),
    mNames(names)
{
    if constexpr(N > 0)
    {
        if(elementSpacesHeight < 0)
        {
            elementSpacesHeight = 0;
        }

        float const &&spaceBetween = (frame.getSize().y - elementSpacesHeight * N) / (N + 1);
        float const &&yJump = elementSpacesHeight + spaceBetween;
        float yPos = frame.getPosition().y + spaceBetween;

        for(size_t i = 0; i < N; ++i)
        {
            mElementSpaces[i].setSize(sf::Vector2f(frame.getSize().x, elementSpacesHeight));
            mElementSpaces[i].setPosition(frame.getPosition().x, yPos);
            mElementSpaces[i].setFillColor(sf::Color(77, 164, 240));

            mImages[i].setPosition(mElementSpaces[i].getPosition() + sf::Vector2f(5.f, 0.f));
            mImages[i].scale(elementSpacesHeight / mImages[i].getGlobalBounds().width,
                             elementSpacesHeight / mImages[i].getGlobalBounds().height);

            mNames[i].setOrigin(mNames[i].getOrigin() + sf::Vector2f(0.f, mNames[i].getGlobalBounds().height / 2.f));
            mNames[i].setPosition(
                  mImages[i].getPosition()
                + sf::Vector2f(mImages[i].getGlobalBounds().width + 3.f, mImages[i].getGlobalBounds().height / 2.f));

            yPos += yJump;
        }
    }
}


template<std::size_t N>
void gs::Table<N>::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mFrame);

    if constexpr(N > 0)
    {
        for(size_t i = 0; i < N; ++i)
        {
            target.draw(mElementSpaces[i]);
            target.draw(mImages[i]);
            target.draw(mNames[i]);
        }
    }
}


template<std::size_t N>
short gs::Table<N>::update(sf::View const &view) const
{
    if constexpr(N > 0)
    {
        if(gs::App::mouse.isButtonPressed(gs::App::mainButton))
        {
            sf::Vector2f const mousePos = gs::App::mouse.getCurrentCoords(view);

            for(size_t i = 0; i < N; ++i)
            {
                if(mElementSpaces[i].getGlobalBounds().contains(mousePos))
                {
                    return static_cast<short>(i);
                }
            }
        }
    }

    return -1;
}
