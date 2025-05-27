#include "App.h"

#include "InterfacePMS.h"
#include "ProjectileMotionSimulation.h"


using namespace std;


gs::App::App():
    mEvent(),
    mEndIter(),
    mStartIter()
{
    this->addEvent(sf::Event::Closed, bind(&App::pollEventClosed, ref(*this), placeholders::_1));
    this->addEvent(sf::Event::Resized, bind(&App::pollEventResized, ref(*this), placeholders::_1));

    mpf_set_default_prec(defMpfClassPrec);

    mArialFont.loadFromFile(mArialFontPath.string());

    mCheckboxUnchecked.loadFromFile(mCheckboxesPath.string(), {sf::Vector2i(0, 0), singleCheckboxDims});
    mCheckboxChecked.loadFromFile(mCheckboxesPath.string(), {sf::Vector2i(singleCheckboxDims.x + 1, 0), singleCheckboxDims});
    mCelestialBodies.loadFromFile(mCelestialBodiesPath.string());

    mWindow.setSize(mDefWindowDims);
    mWindow.setPosition(sf::Vector2i(maxWindowDims - mDefWindowDims) / 2);

    mPMS = make_unique<gs::ProjectileMotionSimulation>(),
    mInterface = make_unique<gs::InterfacePMS>(*mPMS);

    mPMS->assignInterface(*mInterface);
}


gs::App & gs::App::getInstance()
{
    static App instance;

    return instance;
}


void gs::App::run()
{
    mElapsedIter_us = chrono::microseconds{0};
    mStartIter = chrono::steady_clock::now();

    while(mWindow.isOpen())
    {
        this->pollEvents();
        this->update();
        this->render();

        mEndIter = chrono::steady_clock::now();
        mElapsedIter_us = chrono::duration_cast<chrono::microseconds>(mEndIter - mStartIter);
        mStartIter = mEndIter;
    }
}

void gs::App::pollEvents()
{
    while(mWindow.pollEvent(mEvent))
    {
        gs::EventsHandlerWithAccess::handleEvent(mEvent);
    }
}


void gs::App::update()
{
    mMouse.update();

    mPMS->update();
    mInterface->update();
}


void gs::App::render()
{
    mWindow.clear();

    mWindow.draw(*mPMS);
    mWindow.draw(*mInterface);

    mWindow.display();
}

void gs::App::pollEventClosed(sf::Event const &event)
{
    this->exit();
}

void gs::App::pollEventResized(sf::Event const &event)
{
    this->resizeWindow(event.size);
}


void gs::App::exit()
{
    mWindow.close();
}

void gs::App::resizeWindow(sf::Event::SizeEvent const &sizeEvent)
{
    if(sizeEvent.width > App::maxWindowDims.x)
    {
        if(sizeEvent.height > App::maxWindowDims.y)
        {
            mWindow.setSize(sf::Vector2u(App::maxWindowDims.x, App::maxWindowDims.y));
        }
        else
        {
            mWindow.setSize(sf::Vector2u(App::maxWindowDims.x, App::window.getSize().y));
        }
    }
    else if(sizeEvent.height > App::maxWindowDims.y)
    {
        mWindow.setSize(sf::Vector2u(mWindow.getSize().x, App::maxWindowDims.y));
    }
}


std::tm gs::App::getCurrentLocalTime()
{
    time_t const &&nowTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm const currLocalTime = *localtime(&nowTime);

    return currLocalTime;
}
