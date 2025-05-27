#ifndef PROJECTILEMOTIONSIMULATION_H
#define PROJECTILEMOTIONSIMULATION_H

#include "App.h"
#include "BinarySwitch.h"
#include "Guides.h"
#include "Curve.h"
#include "EventsHandler.h"
#include "Graph.h"
#include "LogicalFunctor.h"
#include "ProjectileMotionObject.h"
#include "ToFile_t.h"
#include "UpdateCase.h"

#include <gmpxx.h>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

#include <array>
#include <chrono>
#include <functional>
#include <set>
#include <vector>


namespace gs
{
class ProjectileMotionSimulation final: public gs::EventsHandler, public sf::Drawable
{
private:
    static constexpr char const * const mScreenshotsDirName = "screenshots";
    static constexpr unsigned short mCurvePointsCount = 50;
    static constexpr sf::Keyboard::Key
        startKey = sf::Keyboard::S,
        resetKey = sf::Keyboard::R,
        pauseKey = sf::Keyboard::P,
        exportDataKey = sf::Keyboard::E,
        screenshotKey = sf::Keyboard::I;

    bool mLeftButtonClickFlag = false;
    bool mPhysics = false;
    bool mStartFlag = false;
    bool mResetFlag = false;
    bool mIsExportingToCSV = false;

    gs::InterfacePMS *mIpmsPtr;

    mpf_class mTimeMultiplier;

    sf::RectangleShape mMainBackground, mGraphBackground;
    sf::View mMainBackgroundView, mGraphBackgroundView, mGraphView, mPmoView;

    gs::Graph mGraph;
    gs::ProjectileMotionObject mPMO;
    gs::Guides mGuides;
    gs::Curve<mCurvePointsCount> mCurve = {sf::Color::Blue};

    gs::Vector mVectorX, mVectorY, mVectorR;

    using OrderedFuncPair_t =
        std::pair<int, std::function<void (sf::RenderTarget &, sf::RenderStates const &)>>;

    OrderedFuncPair_t
        drawInterfaceBackgroundPair, drawGraphBackgroundPair,
        drawAxesPair, drawPMOPair, drawGradutionsPair, drawGraphKeysPair,
        drawCurvePair, drawGuidesPair, drawVectorXPair, drawVectorYPair,
        drawVectorRPair, drawGraphValuesPair;

    std::set<
        OrderedFuncPair_t const *,
        std::function<bool(OrderedFuncPair_t const *, OrderedFuncPair_t const *)>
    > mObjectsToRender {
        [] (OrderedFuncPair_t const *a, OrderedFuncPair_t const *b) {return a->first > b->first;}
    };

public:
    gs::BinarySwitch const
        xVecVisibilitySwitch = {
            [&] () {mObjectsToRender.erase(&drawVectorXPair);},
            [&] () {mObjectsToRender.insert(&drawVectorXPair);},
            true},
        yVecVisibilitySwitch = {
            [&] () {mObjectsToRender.erase(&drawVectorYPair);},
            [&] () {mObjectsToRender.insert(&drawVectorYPair);},
            true},
        rVecVisibilitySwitch = {
            [&] () {mObjectsToRender.erase(&drawVectorRPair);},
            [&] () {mObjectsToRender.insert(&drawVectorRPair);},
            true},
        guidesVisibilitySwitch = {
            [&] () {mObjectsToRender.erase(&drawGuidesPair);},
            [&] () {mObjectsToRender.insert(&drawGuidesPair);},
            true},
        curveVisibilitySwitch = {
            [&] () {mObjectsToRender.erase(&drawCurvePair);},
            [&] () {mObjectsToRender.insert(&drawCurvePair);},
            true};

private:
    using sfK = sf::Keyboard;
    std::array<gs::LogicalFunctor, 5> mPressFunctors = {
        gs::LogicalFunctor{
            [] () {return gs::App::window.hasFocus() && sfK::isKeyPressed(startKey);},
            std::bind(&ProjectileMotionSimulation::start, this)},
        gs::LogicalFunctor{
            [] () {return  gs::App::window.hasFocus() &&sfK::isKeyPressed(resetKey);},
            std::bind(&ProjectileMotionSimulation::reset, this)},
        gs::LogicalFunctor{
            [] () {return gs::App::window.hasFocus() && sfK::isKeyPressed(pauseKey);},
            std::bind(&ProjectileMotionSimulation::pause, this)},

        gs::LogicalFunctor{
            [] () {return gs::App::window.hasFocus() && sfK::isKeyPressed(exportDataKey);},
            std::bind(&ProjectileMotionSimulation::exportDataToCSVAsync, this)},

        gs::LogicalFunctor{
            [] () {return gs::App::window.hasFocus() && sfK::isKeyPressed(screenshotKey);},
            std::bind(&ProjectileMotionSimulation::exportViewToPNGAsync, this)}
    };

    using PMO = gs::ProjectileMotionObject;
    gs::ToFile_t mToCSV = {
        std::string("measurements.csv"),
        make_pair(mpf_class("0.00"), mPMO.getFlightTime()),
        10,
        std::vector<std::function<mpf_class (mpf_class const &)>>{
            std::bind(&PMO::getVAfterTime, &mPMO, placeholders::_1),
            std::bind(&PMO::getVxAfterTime, &mPMO, placeholders::_1),
            std::bind(&PMO::getVyAfterTime, &mPMO, placeholders::_1),
            std::bind(&PMO::getHAfterTime, &mPMO, placeholders::_1),
            std::bind(&PMO::getDistanceAfterTime, &mPMO, placeholders::_1),
            std::bind(&PMO::getAngleAfterTime, &mPMO, placeholders::_1)},
        std::vector<std::string>{"V", "Vx", "Vy", "h", "d", "alpha"},
        true,
        2
    };

    std::map<gs::UpdateCase, void (ProjectileMotionSimulation::* const) ()
        > mVectorUpdateFunctions = {
        {gs::UpdateCase::motion, &updateVectorsAtMotion},
        {gs::UpdateCase::resize, &updateVectorsAtResize},
        {gs::UpdateCase::paramsChange, &updateVectorsAtParamsChange}
    };


public:
    explicit ProjectileMotionSimulation();
    void assignInterface(gs::InterfacePMS &interfacePMS);


private:
    void prepareDirectories();
    void prepareObjectsToRender();
    void prepareViews();
    void prepareInterfaceBackground();
    void prepareGraphBackground();
    void prepareVectors();
    void prepareVectorX();
    void prepareVectorY();
    void prepareVectorR();
    void prepareObjectStartValues();


private:
    void pollEventResized(sf::Event const &event);
    void pollEventMouseButtonPressed(sf::Event const &event);

public:
    void update();

private:
    void updatePMO(mpf_class const &time_s);
    void updateScale();
    void moveSimulationView();
    void updateCurve();
    void updateGuides();
    void updateVectors(gs::UpdateCase const updateCase);
        void updateVectorsAtMotion();
        void updateVectorsAtResize();
        void updateVectorsAtParamsChange();

    float getHeightUnitsPerUnit();

public:
    gs::ProjectileMotionObject const & getPMO();

    void setPhysics(bool const boolean);

    void start();
    void reset();
    void pause();
        void stop();
        void resume();

    void setAngle0InDeg(mpf_class const &angle0);
    void setHeight0(mpf_class const &height0);
    void setResVelocity0(mpf_class const &v0);
    void setAcc(mpf_class const &acc0);
    void setTimeMultiplier(mpf_class const &timeMultiplier);
    void setIntervalsCount(unsigned short const intervalsCount);
    void setSimulationAtGiventTime(mpf_class const &time_s);

    void switchScallingMethod();

    void setExportedDataPrecision(short const precision);

    void exportDataToCSVAsync();
    void exportViewToPNGAsync();

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
        void drawInterfaceBackground(sf::RenderTarget &target, sf::RenderStates const &states) const;
        void drawGraphBackground    (sf::RenderTarget &target, sf::RenderStates const &states) const;
        void drawAxes               (sf::RenderTarget &target, sf::RenderStates const &states) const;
        void drawPMO                (sf::RenderTarget &target, sf::RenderStates const &states) const;
        void drawGradutions         (sf::RenderTarget &target, sf::RenderStates const &states) const;
        void drawGraphKeys          (sf::RenderTarget &target, sf::RenderStates const &states) const;
        void drawCurve              (sf::RenderTarget &target, sf::RenderStates const &states) const;
        void drawGuides             (sf::RenderTarget &target, sf::RenderStates const &states) const;
        void drawVectorX            (sf::RenderTarget &target, sf::RenderStates const &states) const;
        void drawVectorY            (sf::RenderTarget &target, sf::RenderStates const &states) const;
        void drawVectorR            (sf::RenderTarget &target, sf::RenderStates const &states) const;
        void drawGraphValues        (sf::RenderTarget &target, sf::RenderStates const &states) const;
        void drawInterface          (sf::RenderTarget &target, sf::RenderStates const &states) const;

    sf::FloatRect getNotRotatedViewBounds(sf::View const &view) const;
};

}

#endif // PROJECTILEMOTIONSIMULATION_H









