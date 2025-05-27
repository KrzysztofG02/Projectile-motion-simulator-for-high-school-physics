#ifndef APP_H
#define APP_H

#include "EventsHandler.h"
#include "Mouse.h"

#include <gmpxx.h>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <chrono>
#include <filesystem>
#include <memory>


namespace gs
{

class InterfacePMS;
class ProjectileMotionSimulation;

class App final: private EventsHandler ///Singleton
{
public:
    //open-access static members
    static constexpr char const * const programTitle = "Projectile Motion Simulator";
    static inline unsigned short const defMpfClassPrec = 256u;
    static inline sf::Vector2u const maxWindowDims = [] () -> sf::Vector2u {sf::VideoMode dm = sf::VideoMode::getDesktopMode(); return {dm.width, dm.height};}();
    static inline sf::Mouse::Button const mainButton = sf::Mouse::Left;
    static inline mpf_class const terminalV = {"299792457.99", defMpfClassPrec};
    static inline sf::Vector2i const singleCheckboxDims = {478, 478};
    static inline sf::Vector2i const singleCelestialBodyDims = {1000, 1000};
    static inline unsigned short const celestialBodyCount = 11;
    static inline std::filesystem::path const screenshotsDir = "screenshots";
    static inline std::filesystem::path const assetsDir = "assets";
    //open-access static members/

private:
    //private static members
    static inline sf::Vector2u const mDefWindowDims = {1280u, 720u};
    static inline std::filesystem::path const mArialFontFile = "arial.ttf";
    static inline std::filesystem::path const mCheckboxesFile = "checkboxes.png";
    static inline std::filesystem::path const mCelestialBodiesFile = "solar_system.png";
    static inline std::filesystem::path const mArialFontPath = assetsDir / mArialFontFile;
    static inline std::filesystem::path const mCheckboxesPath = assetsDir / mCheckboxesFile;
    static inline std::filesystem::path const mCelestialBodiesPath = assetsDir / mCelestialBodiesFile;
    //private static members/

private:
    //static members with controlled access
    static inline sf::Font mArialFont = {};
    static inline sf::Texture mCheckboxUnchecked = {};
    static inline sf::Texture mCheckboxChecked = {};
    static inline sf::Texture mCelestialBodies = {};
    static inline sf::RenderWindow mWindow = {sf::VideoMode(), programTitle};
    static inline gs::Mouse mMouse = gs::Mouse::getInstance(mWindow, {sf::Mouse::Left});
    static inline std::chrono::microseconds mElapsedIter_us = {};

public:
    static inline sf::Font const &arialFont = mArialFont;
    static inline sf::Texture const &checkboxUnchecked = mCheckboxUnchecked;
    static inline sf::Texture const &checkboxChecked = mCheckboxChecked;
    static inline sf::Texture const &celestialBodies = mCelestialBodies;
    static inline sf::RenderWindow const &window = mWindow;
    static inline gs::Mouse const &mouse = mMouse;
    static inline std::chrono::microseconds const &elapsedIter_us = mElapsedIter_us;
    //static members with controlled access/


private:
    sf::Event mEvent;
    std::chrono::steady_clock::time_point mEndIter;
    std::chrono::steady_clock::time_point mStartIter;

    std::unique_ptr<ProjectileMotionSimulation> mPMS;
    std::unique_ptr<InterfacePMS> mInterface;


private:
    App();


public:
    static App & getInstance();


public:
    void run();


private:
    void pollEvents();
    void update();
    void render();

    void pollEventClosed(sf::Event const &event);
    void pollEventResized(sf::Event const &event);

    void exit();
    void resizeWindow(sf::Event::SizeEvent const &sizeEvent);

public:
    static std::tm getCurrentLocalTime();
};

}

#endif // APP_H
