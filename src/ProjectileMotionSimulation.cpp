#include "ProjectileMotionSimulation.h"

#include "App.h"
#include "InterfacePMS.h"

#include <filesystem>
#include <future>
#include <iomanip>
#include <sstream>
#include <thread>

using namespace std;


gs::ProjectileMotionSimulation::ProjectileMotionSimulation():
    mTimeMultiplier("1.00"),

    mMainBackground(sf::Vector2f(gs::App::maxWindowDims.x, gs::App::maxWindowDims.y)),
    mGraphBackground(sf::Vector2f(gs::App::maxWindowDims.x, gs::App::maxWindowDims.y)),

    mMainBackgroundView(
        sf::FloatRect(0.f, 0.f, gs::App::window.getSize().x, gs::App::window.getSize().y)),
    mGraphBackgroundView(
         sf::FloatRect(0.f, 0.f, gs::App::window.getSize().x, gs::App::window.getSize().y * 0.7f)),
    mGraphView(mGraphBackgroundView),
    mPmoView(mGraphBackgroundView),

    mGraph(mGraphView),
    mPMO(
         mpf_class("9.81"),
         sf::Vector2<mpf_class>(mpf_class(mGraph.getAxes().y.getPointOfApp().x),
                                mpf_class(mGraph.getAxes().x.getPointOfApp().y)),
         11.f),
    mGuides(sf::Vector3f(mGraph.getAxes().y.getPointOfApp().x,
                         mPMO.getCurrentPosition().x.get_d(),
                         mPMO.getCurrentPosition().y.get_d()),
            sf::Vector3f(mPMO.getCurrentPosition().x.get_d(),
                         mPMO.getCurrentPosition().y.get_d(),
                         mGraph.getAxes().x.getPointOfApp().y),
            sf::Color(224, 131, 9)),

    mVectorX(sf::Vector2f(mPMO.getPosition0().x.get_d(), mPMO.getPosition0().x.get_d()),
             mPMO.getVx().get_d(), 1.f, 0,
             4, sf::Color(0, 150, 0, 200), 0, false),
    mVectorY(sf::Vector2f(mPMO.getPosition0().x.get_d(), mPMO.getPosition0().x.get_d()),
             mPMO.getVy0().get_d(), 1.f, mPMO.getVy0Sense() == gs::VectorSense::up ? 90 : 270,
             4, sf::Color(255, 0, 255, 200), 0, false),
    mVectorR(sf::Vector2f(mPMO.getPosition0().x.get_d(), mPMO.getPosition0().x.get_d()),
             mPMO.getV0().get_d(), 1.f, gs::toDegrees(mPMO.getAngle0()).get_d(),
             4, sf::Color(19, 141, 148, 200), 0, false)
{
    this->addEvent(
        sf::Event::Resized,
        bind(&ProjectileMotionSimulation::pollEventResized, this, placeholders::_1));

    this->prepareDirectories();
    this->prepareObjectsToRender();
    this->prepareViews();
    this->prepareInterfaceBackground();
    this->prepareGraphBackground();
    this->prepareVectors();

    this->prepareObjectStartValues();
}

void gs::ProjectileMotionSimulation::assignInterface(gs::InterfacePMS &interfacePMS)
{
    mIpmsPtr = &interfacePMS;
}


void gs::ProjectileMotionSimulation::prepareDirectories()
{
    if(!filesystem::exists(mScreenshotsDirName))
    {
        filesystem::create_directory(mScreenshotsDirName);
    }
}

void gs::ProjectileMotionSimulation::prepareObjectsToRender()
{
    unsigned short i = 0;

    #define BIND_MEMBER_FN(fn) bind(&ProjectileMotionSimulation::fn, this, placeholders::_1, placeholders::_2)

    drawGraphValuesPair         = {i++, BIND_MEMBER_FN(drawGraphValues)};
    drawVectorRPair             = {i++, BIND_MEMBER_FN(drawVectorR)};
    drawVectorYPair             = {i++, BIND_MEMBER_FN(drawVectorY)};
    drawVectorXPair             = {i++, BIND_MEMBER_FN(drawVectorX)};
    drawGuidesPair              = {i++, BIND_MEMBER_FN(drawGuides)};
    drawCurvePair               = {i++, BIND_MEMBER_FN(drawCurve)};
    drawGraphKeysPair           = {i++, BIND_MEMBER_FN(drawGraphKeys)};
    drawGradutionsPair          = {i++, BIND_MEMBER_FN(drawGradutions)};
    drawPMOPair                 = {i++, BIND_MEMBER_FN(drawPMO)};
    drawAxesPair                = {i++, BIND_MEMBER_FN(drawAxes)};
    drawGraphBackgroundPair     = {i++, BIND_MEMBER_FN(drawGraphBackground)};
    drawInterfaceBackgroundPair = {i++, BIND_MEMBER_FN(drawInterfaceBackground)};

    mObjectsToRender = {
        &drawInterfaceBackgroundPair, &drawGraphBackgroundPair, &drawAxesPair, &drawPMOPair, &drawGradutionsPair,
        &drawGraphKeysPair, &drawCurvePair, &drawGuidesPair, &drawVectorXPair, &drawVectorYPair, &drawVectorRPair,
        &drawGraphValuesPair};
}

void gs::ProjectileMotionSimulation::prepareViews()
{
    mMainBackgroundView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

    mGraphBackgroundView.setViewport(sf::FloatRect(0.f, 0.15f, 1.f, 0.7f));

    mGraphView.setViewport(sf::FloatRect(0.f, 0.15f, 1.f, 0.7f));
    mPmoView.setViewport(sf::FloatRect(0.f, 0.15f, 1.f, 0.7f));
}

void gs::ProjectileMotionSimulation::prepareInterfaceBackground()
{
    mMainBackground.setFillColor(sf::Color(199, 242, 228));
    mMainBackground.setPosition(0.f, 0.f);
}

void gs::ProjectileMotionSimulation::prepareGraphBackground()
{
    mGraphBackground.setFillColor(sf::Color(240, 240, 240));
    mGraphBackground.setPosition(0.f, 0.f);
}

void gs::ProjectileMotionSimulation::prepareVectors()
{
    this->prepareVectorX();
    this->prepareVectorY();
    this->prepareVectorR();
}

void gs::ProjectileMotionSimulation::prepareVectorX()
{
    mVectorX.name.setFont(gs::App::arialFont);
    mVectorX.name.setString("Vx");
    mVectorX.name.setCharacterSize(100);
    mVectorX.name.setFillColor(sf::Color(0, 150, 0));
    mVectorX.name.setPosition(sf::Vector2f(mPMO.getPosition0().x.get_d(), mPMO.getPosition0().x.get_d()) + mPMO.getRadius() * sf::Vector2f(2.f, -2.f));
}

void gs::ProjectileMotionSimulation::prepareVectorY()
{
    mVectorY.name.setFont(gs::App::arialFont);
    mVectorY.name.setString("Vy");
    mVectorY.name.setCharacterSize(100);
    mVectorY.name.setFillColor(sf::Color(255, 0, 255));
    mVectorY.name.setPosition(sf::Vector2f(mPMO.getPosition0().x.get_d(), mPMO.getPosition0().x.get_d()) + mPMO.getRadius() * sf::Vector2f(-1.5f, 1.25f));
}

void gs::ProjectileMotionSimulation::prepareVectorR()
{
    mVectorR.name.setFont(gs::App::arialFont);
    mVectorR.name.setString("V");
    mVectorR.name.setCharacterSize(100);
    mVectorR.name.setFillColor(sf::Color(19, 141, 148));
    mVectorR.name.setPosition(sf::Vector2f(mPMO.getPosition0().x.get_d(), mPMO.getPosition0().x.get_d()) + mPMO.getRadius() * sf::Vector2f(-2.f, -2.f));
}

void gs::ProjectileMotionSimulation::prepareObjectStartValues()
{
    this->setAngle0InDeg(mpf_class("45.00"));
    this->setHeight0(mpf_class("5.00"));
    this->setResVelocity0(mpf_class("10.00"));
}


void gs::ProjectileMotionSimulation::pollEventResized(sf::Event const &event)
{
    mMainBackgroundView.reset(sf::FloatRect(0.f, 0.f, gs::App::window.getSize().x, gs::App::window.getSize().y));

    mGraphView.reset(sf::FloatRect(0.f, 0.f, gs::App::window.getSize().x, gs::App::window.getSize().y * 0.7f));
    mPmoView.reset(sf::FloatRect(0.f, 0.f, gs::App::window.getSize().x, gs::App::window.getSize().y * 0.7f));

    mGraph.updateAtResize(mGraphView);

    mPmoView.setCenter(mGraphView.getCenter());

    this->updateScale();

    this->updateGuides();
    this->updateVectors(gs::UpdateCase::resize);
    this->updateCurve();
}


void gs::ProjectileMotionSimulation::update()
{
    if(mPhysics)
    {
        if(!mPMO.hasEquilibrium())
        {
            this->updatePMO(mpf_class(to_string(gs::App::elapsedIter_us.count())) * mTimeMultiplier / mpf_class("1000000.00"));
        }
        else
        {
            this->setPhysics(false);
            mStartFlag = false;
        }
    }

    if(mLeftButtonClickFlag)
    {
        if(gs::App::mouse.isButtonPressed(gs::App::mainButton) && gs::App::window.hasFocus())
        {
            if(gs::App::mouse.getCoordsDelta(mGraphView).y != 0)
            {
                this->moveSimulationView();
            }
        }
        else
        {
            mLeftButtonClickFlag = false;
        }
    }
    else if(   !gs::App::mouse.wasButtonPressed(gs::App::mainButton)
            && gs::App::mouse.isButtonPressed(gs::App::mainButton)
            && this->getNotRotatedViewBounds(mGraphView).contains(gs::App::mouse.getCurrentCoords(mGraphView)))
    {
        mLeftButtonClickFlag = true;
    }

    for(auto &pressFunctor : mPressFunctors)
    {
        pressFunctor();
    }
}

void gs::ProjectileMotionSimulation::updatePMO(mpf_class const &time_s)
{
    mPMO.update(time_s);

    this->updateGuides();
    this->updateVectors(gs::UpdateCase::motion);
}

void gs::ProjectileMotionSimulation::updateScale()
{
    mGraph.updateScale(sf::Vector2<mpf_class>(mPMO.getReach(), mPMO.getMaxH()), mGraphView);
    mPMO.setScale(mGraph.getScale());
}


void gs::ProjectileMotionSimulation::moveSimulationView()
{
    float const viewCenterY = mGraphView.getCenter().y;
    float const &&viewHalfHeight = mGraphView.getSize().y / 2.f;
    sf::Vector2f const delta = gs::App::mouse.getCoordsDelta(mGraphView);

    if(   (delta.y > 0 && mGraph.getAxes().x.getPointOfApp().y + gs::Graph::mGraphMargin.bottom < viewCenterY + viewHalfHeight - delta.y)
       || (delta.y < 0 && mGraph.getAxes().x.getPointOfApp().y - gs::Graph::mGraphMargin.top > viewCenterY - viewHalfHeight - delta.y))
    {
        mGraphView.move(0.f, -delta.y);
        mPmoView.move(0.f, -delta.y);

        mGraph.updateAtViewMove(mGraphView);
    }
}

void gs::ProjectileMotionSimulation::updateCurve()
{
    size_t const &&lastIdx = mCurve.getPointCount() - 1;
    mpf_class const &&timePeriod = mPMO.getFlightTime() / mpf_class(mCurve.getPointCount());
    sf::Vector2<mpf_class> const pmoStartPos = mPMO.getPosition0();
    sf::Vector2<mpf_class> const pmoEndPos = mPMO.getPositionAfterTime(mPMO.getFlightTime());

    mCurve[0].position = sf::Vector2f(pmoStartPos.x.get_d(), pmoStartPos.y.get_d());
    mCurve[lastIdx].position = {static_cast<float>(pmoEndPos.x.get_d()), static_cast<float>(pmoEndPos.y.get_d())};

    for(size_t i = 1; i < lastIdx; ++i)
    {
        sf::Vector2<mpf_class> const pmoPosAfterTime = mPMO.getPositionAfterTime(i * timePeriod);
        mCurve[i].position = {static_cast<float>(pmoPosAfterTime.x.get_d()), static_cast<float>(pmoPosAfterTime.y.get_d())};
    }
}

float gs::ProjectileMotionSimulation::getHeightUnitsPerUnit()
{
    mpf_class const maxVx = mPMO.getVx(), fT = mPMO.getFlightTime();
    mpf_class Vy0 = mPMO.getVy0(), VyEnd = mPMO.getVyAfterTime(fT);

    mpf_abs(Vy0.get_mpf_t(), Vy0.get_mpf_t());
    mpf_abs(VyEnd.get_mpf_t(), VyEnd.get_mpf_t());

    mpf_class const maxVy = max(Vy0, VyEnd);
    sf::Vector2<double> const viewSize(
        static_cast<double>(mGraphView.getSize().x - mpf_class(mPMO.getPositionAfterTime(fT).x - mPMO.getZeroPoint().x).get_d() - gs::Graph::mGraphMargin.left),
        static_cast<double>(mGraphView.getSize().y - mpf_class(mPMO.getZeroPoint().y - mPMO.getPositionAfterTime(fT).y).get_d() - gs::Graph::mGraphMargin.bottom));

    if(maxVx.get_d() / viewSize.x > maxVy.get_d() / viewSize.y)
    {
        return static_cast<float>(0.4 * viewSize.x / maxVx.get_d());
    }

    return static_cast<float>(0.4 * viewSize.y / maxVy.get_d());
}

void gs::ProjectileMotionSimulation::updateGuides()
{
    mGuides.setHLineCoords(mGraph.getAxes().y.getPointOfApp().x, mPMO.getCurrentPosition().x.get_d(), mPMO.getCurrentPosition().y.get_d());
    mGuides.setVLineCoords(mPMO.getCurrentPosition().x.get_d(), mPMO.getCurrentPosition().y.get_d(), mGraph.getAxes().x.getPointOfApp().y);
}

void gs::ProjectileMotionSimulation::updateVectors(gs::UpdateCase const updateCase)
{
    (this->*mVectorUpdateFunctions[updateCase])();
}

void gs::ProjectileMotionSimulation::updateVectorsAtMotion()
{
    sf::Vector2f const pmoCurrPos = {static_cast<float>(mPMO.getCurrentPosition().x.get_d()), static_cast<float>(mPMO.getCurrentPosition().y.get_d())};

    mVectorX.setPointOfApp(pmoCurrPos);

    mVectorY.setPointOfApp(pmoCurrPos);
    mVectorY.setUnits(fabs(mPMO.getCurrentVy().get_d()));
    mVectorY.setRotation(mPMO.getCurrentVySense() == gs::VectorSense::up ? 90 : 270);

    mVectorR.setPointOfApp(pmoCurrPos);
    mVectorR.setUnits(fabs(mPMO.getCurrentV().get_d()));
    mVectorR.setRotation(gs::toDegrees(mPMO.getCurrentAngle().get_d()));
}

void gs::ProjectileMotionSimulation::updateVectorsAtResize()
{
    sf::Vector2f const pmoCurrPos = {static_cast<float>(mPMO.getCurrentPosition().x.get_d()), static_cast<float>(mPMO.getCurrentPosition().y.get_d())};
    float const heightUnitsPerUnit = this->getHeightUnitsPerUnit();

    mVectorX.setPointOfApp(pmoCurrPos);
    mVectorX.setHeightUnitsPerUnit(heightUnitsPerUnit);

    mVectorY.setPointOfApp(pmoCurrPos);
    mVectorY.setHeightUnitsPerUnit(heightUnitsPerUnit);

    mVectorR.setPointOfApp(pmoCurrPos);
    mVectorR.setHeightUnitsPerUnit(heightUnitsPerUnit);
}

void gs::ProjectileMotionSimulation::updateVectorsAtParamsChange()
{
    sf::Vector2f const pmoPos0 = {static_cast<float>(mPMO.getPosition0().x.get_d()), static_cast<float>(mPMO.getPosition0().y.get_d())};
    float const heightUnitsPerUnit = this->getHeightUnitsPerUnit();

    mVectorX.setPointOfApp(pmoPos0);
    mVectorX.setHeightUnitsPerUnit(heightUnitsPerUnit);
    mVectorX.setUnits(mPMO.getVx().get_d());

    mVectorY.setPointOfApp(pmoPos0);
    mVectorY.setUnits(fabs(mPMO.getVy0().get_d()));
    mVectorY.setHeightUnitsPerUnit(heightUnitsPerUnit);
    mVectorY.setRotation(mPMO.getVy0Sense() == gs::VectorSense::up ? 90 : 270);

    mVectorR.setPointOfApp(pmoPos0);
    mVectorR.setUnits(fabs(mPMO.getV0().get_d()));
    mVectorR.setHeightUnitsPerUnit(heightUnitsPerUnit);
    mVectorR.setRotation(gs::toDegrees(mPMO.getAngle0()).get_d());
}



gs::ProjectileMotionObject const & gs::ProjectileMotionSimulation::getPMO()
{
    return mPMO;
}


void gs::ProjectileMotionSimulation::setPhysics(bool const isEnabled)
{
    mPhysics = isEnabled;
}


void gs::ProjectileMotionSimulation::start()
{
    if(mPMO.hasEquilibrium())
    {
        this->reset();
    }

    this->setPhysics(true);

    mStartFlag = true;
    mResetFlag = false;
}

void gs::ProjectileMotionSimulation::reset()
{
    this->setPhysics(false);

    mPMO.reset();
    this->updateVectors(UpdateCase::paramsChange);
    this->updateGuides();

    mStartFlag = false;
    mResetFlag = true;
}

void gs::ProjectileMotionSimulation::pause()
{
    if(mStartFlag)
    {
        if(mPhysics)
        {
            this->stop();
        }
        else
        {
            this->resume();
        }
    }
}

void gs::ProjectileMotionSimulation::stop()
{
    this->setPhysics(false);
}

void gs::ProjectileMotionSimulation::resume()
{
    this->setPhysics(true);
}


void gs::ProjectileMotionSimulation::setAngle0InDeg(mpf_class const &angle0)
{
    mPMO.setAngle0(gs::toRadians(angle0));

    this->updateScale();
    this->reset();
    this->updateVectors(UpdateCase::paramsChange);
    this->updateCurve();

    mToCSV.setTimeRange(make_pair(mpf_class("0.00"), mPMO.getFlightTime()));
}

void gs::ProjectileMotionSimulation::setHeight0(mpf_class const &height0)
{
    mPMO.setH0(height0);

    this->updateScale();
    this->reset();
    this->updateVectors(UpdateCase::paramsChange);
    this->updateCurve();

    mToCSV.setTimeRange(make_pair(mpf_class("0.00"), mPMO.getFlightTime()));
}

void gs::ProjectileMotionSimulation::setResVelocity0(mpf_class const &v0)
{
    mPMO.setV0(v0);

    this->updateScale();
    this->reset();
    this->updateVectors(UpdateCase::paramsChange);
    this->updateCurve();

    mToCSV.setTimeRange(make_pair(mpf_class("0.00"), mPMO.getFlightTime()));
}

void gs::ProjectileMotionSimulation::setAcc(mpf_class const &acc0)
{
    mPMO.setAcc(acc0);

    this->updateScale();
    this->reset();
    this->updateVectors(UpdateCase::paramsChange);
    this->updateCurve();

    mToCSV.setTimeRange(make_pair(mpf_class("0.00"), mPMO.getFlightTime()));
}

void gs::ProjectileMotionSimulation::setTimeMultiplier(mpf_class const &timeMultiplier)
{
    mTimeMultiplier = timeMultiplier;
}

void gs::ProjectileMotionSimulation::setIntervalsCount(unsigned short const intervalsCount)
{
    mToCSV.setIntervalsCount(intervalsCount);
}

void gs::ProjectileMotionSimulation::setSimulationAtGiventTime(mpf_class const &time_s)
{
    this->stop();
    mPMO.reset();
    mStartFlag = true;
    this->updatePMO(time_s);
}


void gs::ProjectileMotionSimulation::switchScallingMethod()
{
    mGraph.switchScalingMethod();

    this->updateScale();

    this->updateGuides();
    this->updateVectors(gs::UpdateCase::resize);
    this->updateCurve();
}


void gs::ProjectileMotionSimulation::setExportedDataPrecision(short const precision)
{
    mToCSV.setPrecision(precision);
}


void gs::ProjectileMotionSimulation::exportDataToCSVAsync()
{
    if(!mIsExportingToCSV)
    {
        gs::RectangleContainer<sf::Text> &csvButton = mIpmsPtr->getToCsvButton();

        csvButton.setEnabled(false);
        mIsExportingToCSV = true;

        thread([&] () {
            mToCSV();
            csvButton.setEnabled(true);
            mIsExportingToCSV = false;
        }).detach();
    }
}

void gs::ProjectileMotionSimulation::exportViewToPNGAsync()
{
    promise<sf::Image> imagePromise;
    future<sf::Image> imageFuture = imagePromise.get_future();

    sf::Texture screenshot;
    screenshot.create(gs::App::window.getSize().x, gs::App::window.getSize().y);
    screenshot.update(gs::App::window);

    imagePromise.set_value(screenshot.copyToImage());

    promise<string> fileNamePromise;
    future<string> fileNameFuture = fileNamePromise.get_future();

    thread fileNameThread([fileNamePromise = move(fileNamePromise)] () mutable {
        tm const &&currLocalTime = gs::App::getCurrentLocalTime();
        ostringstream oss;

        oss << put_time(&currLocalTime, "%Y_%m_%d_%H_%M_%S_");

        fileNamePromise.set_value(
            (gs::App::screenshotsDir / (oss.str()
            + to_string(gs::App::elapsedIter_us.count()) + ".png")).string());
    });

    thread saveThread([fileNameFuture = move(fileNameFuture), imageFuture = move(imageFuture)] () mutable {
        if(sf::Image const && im = imageFuture.get(); !im.saveToFile(fileNameFuture.get()))
        {
            cerr << "Failed to save screenshot: " << fileNameFuture.get() << '\n';
        }
    });

    fileNameThread.detach();
    saveThread.detach();
}


void gs::ProjectileMotionSimulation::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for(OrderedFuncPair_t const * const &orderedFuncPairPtr : mObjectsToRender)
    {
        (orderedFuncPairPtr->second)(target, states);
    }
}

void gs::ProjectileMotionSimulation::drawInterfaceBackground(sf::RenderTarget &target, sf::RenderStates const &states) const
{
    target.setView(mMainBackgroundView);

    target.draw(mMainBackground);
}

void gs::ProjectileMotionSimulation::drawGraphBackground(sf::RenderTarget &target, sf::RenderStates const &states) const
{
    target.setView(mGraphBackgroundView);

    target.draw(mGraphBackground);
}

void gs::ProjectileMotionSimulation::drawAxes(sf::RenderTarget &target, sf::RenderStates const &states) const
{
    target.setView(mGraphView);

    target.draw(mGraph.getAxes().x);
    target.draw(mGraph.getAxes().y);
}

void gs::ProjectileMotionSimulation::drawPMO(sf::RenderTarget &target, sf::RenderStates const &states) const
{
    target.setView(mPmoView);

    target.draw(mPMO);
}

void gs::ProjectileMotionSimulation::drawGradutions(sf::RenderTarget &target, sf::RenderStates const &states) const
{
    target.setView(mGraphView);

    for(sf::Sprite const &graduation : mGraph.getGrads())
    {
        target.draw(graduation);
    }
}

void gs::ProjectileMotionSimulation::drawGraphKeys(sf::RenderTarget &target, sf::RenderStates const &states) const
{
    target.setView(mGraphView);

    target.draw(mGraph.getGraphKeys().x);
    target.draw(mGraph.getGraphKeys().y);
}

void gs::ProjectileMotionSimulation::drawCurve(sf::RenderTarget &target, sf::RenderStates const &states) const
{
    target.setView(mPmoView);

    target.draw(mCurve);
}

void gs::ProjectileMotionSimulation::drawGuides(sf::RenderTarget &target, sf::RenderStates const &states) const
{
    target.setView(mGraphView);

    if(!mPMO.hasEquilibrium())
    {
        target.draw(mGuides);
    }
}

void gs::ProjectileMotionSimulation::drawVectorX(sf::RenderTarget &target, sf::RenderStates const &states) const
{
    target.setView(mGraphView);

    target.draw(mVectorX);
    target.draw(mVectorX.name);
}

void gs::ProjectileMotionSimulation::drawVectorY(sf::RenderTarget &target, sf::RenderStates const &states) const
{
    target.setView(mGraphView);

    target.draw(mVectorY);
    target.draw(mVectorY.name);
}

void gs::ProjectileMotionSimulation::drawVectorR(sf::RenderTarget &target, sf::RenderStates const &states) const
{
    target.setView(mGraphView);

    target.draw(mVectorR);
    target.draw(mVectorR.name);
}

void gs::ProjectileMotionSimulation::drawGraphValues(sf::RenderTarget &target, sf::RenderStates const &states) const
{
    target.setView(mGraphView);

    for(short i = 0; i <= mGraph.getLastVisibleRValueIdx(); ++i)
    {
        target.draw((mGraph.getRValues())[i]);
    }

    for(short i = 0; i <= mGraph.getLastVisibleBValueIdx(); ++i)
    {
        target.draw((mGraph.getBValues())[i]);
    }

    for(short i = 0; i <= mGraph.getLastVisibleTValueIdx(); ++i)
    {
        target.draw((mGraph.getTValues())[i]);
    }
}


sf::FloatRect gs::ProjectileMotionSimulation::getNotRotatedViewBounds(sf::View const &view) const
{
    return sf::FloatRect (view.getCenter() - view.getSize() / 2.f, view.getSize());
}
