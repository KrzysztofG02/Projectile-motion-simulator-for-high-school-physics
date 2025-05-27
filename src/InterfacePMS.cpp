#include "InterfacePMS.h"

#include "App.h"
#include "ProjectileMotionSimulation.h"
#include "StringFunctions.h"

#include <sstream>
#include <thread>

#define Lambd0 [] () {}

using namespace std;


gs::InterfacePMS::InterfacePMS(gs::ProjectileMotionSimulation &pms):
    mPMS(pms),

    mUpperView(sf::FloatRect(0.f, 0.f, gs::App::window.getSize().x, gs::App::window.getSize().y * 0.15f)),
    mLowerView(sf::FloatRect(0.f, 0.f, gs::App::window.getSize().x, gs::App::window.getSize().y * 0.15f)),
    mGlobalView(sf::FloatRect(0.f, 0.f, gs::App::window.getSize().x, gs::App::window.getSize().y)),
    mMpfPMS_methods{
        &ProjectileMotionSimulation::setAngle0InDeg,
        &ProjectileMotionSimulation::setHeight0,
        &ProjectileMotionSimulation::setTimeMultiplier,
        &ProjectileMotionSimulation::setAcc,
        &ProjectileMotionSimulation::setResVelocity0,
        &ProjectileMotionSimulation::setSimulationAtGiventTime},

    //~~~~~~~~~~~~GLOBAL VIEW~~~~~~~~~~~~
    mCelestialBodiesList(),
    mListManagementButton(&mUpperView, Lambd0, Lambd0, Lambd0, Lambd0, L"\u25BA"),
    //~~~~~~~~~~~~GLOBAL VIEW~~~~~~~~~~~~/

    //~~~~~~~~~~~~UPPER VIEW~~~~~~~~~~~~
    //INPUT FIELDS
    mSimParamInputs{
        gs::RectangleInputFieldContainer<mpf_class>(&mUpperView, Lambd0, Lambd0, Lambd0, Lambd0, mpf_class{"-90.00"},                 mpf_class{"90.00"}, mpf_class{"45.00"},  6),
        gs::RectangleInputFieldContainer<mpf_class>(&mUpperView, Lambd0, Lambd0, Lambd0, Lambd0,   mpf_class{"0.00"},            mpf_class{"1000000.00"},  mpf_class{"5.00"}, 10),
        gs::RectangleInputFieldContainer<mpf_class>(&mUpperView, Lambd0, Lambd0, Lambd0, Lambd0,   mpf_class{"0.0000000001"}, mpf_class{"1000000000.00"},  mpf_class{"1.00"}, 12, 10),
        gs::RectangleInputFieldContainer<mpf_class>(&mUpperView, Lambd0, Lambd0, Lambd0, Lambd0,   mpf_class{"0.01"},               mpf_class{"1000.00"},  mpf_class{"9.81"},  7),
        gs::RectangleInputFieldContainer<mpf_class>(&mUpperView, Lambd0, Lambd0, Lambd0, Lambd0,   mpf_class{"0.00"},          mpf_class{"299792457.99"}, mpf_class{"10.00"}, 12),
        gs::RectangleInputFieldContainer<mpf_class>(&mUpperView, Lambd0, Lambd0, Lambd0, Lambd0,   mpf_class{"0.00"},       mpf_class{"100000000000.00"},  mpf_class{"0.00"}, 12)},
    mAngleInput(mSimParamInputs[0]),
    mHeightInput(mSimParamInputs[1]),
    mTimeMultiplierInput(mSimParamInputs[2]),
    mAccInput(mSimParamInputs[3]),
    mVelocityInput(mSimParamInputs[4]),
    mSimulationTimeInput(mSimParamInputs[5]),

    mExportSettingsInput{
        gs::RectangleInputFieldContainer<long>(&mUpperView, Lambd0, Lambd0, Lambd0, Lambd0, 2, 10000, 10, 5),
        gs::RectangleInputFieldContainer<long>(&mUpperView, Lambd0, Lambd0, Lambd0, Lambd0, -1, 99, 2, 2)},
    mIntervalsNumInput(mExportSettingsInput[0]),
    mPrecisionInput(mExportSettingsInput[1]),
    //INPUT FIELDS/

    //INPUT FIELD LABELS
    mInputLabels{
        sf::Text("Start angle [deg]", App::arialFont, 17),
        sf::Text("Start height [m]", App::arialFont, 17),
        sf::Text("Time multiplier", App::arialFont, 17),
        sf::Text("Acceleration [m/s^2]", App::arialFont, 17),
        sf::Text("Start velocity [m/s]", App::arialFont, 17),
        sf::Text("Sim. at given time [s]", App::arialFont, 17),
        sf::Text("Intervals", App::arialFont, 17),
        sf::Text("Precision", App::arialFont, 17)},
    mAngleLabel(mInputLabels[0]),
    mHeightLabel(mInputLabels[1]),
    mTimeMultiplierLabel(mInputLabels[2]),
    mSimulationTimeLabel(mInputLabels[3]),
    mVelocityLabel(mInputLabels[4]),
    mAccLabel(mInputLabels[5]),
    mIntervalsNumLabel(mInputLabels[6]),
    mPrecisionLabel(mInputLabels[7]),
    //INPUT FIELD LABELS/

    //EXPORT BUTTONS
    mExportButtons{
        gs::RectangleTextContainer(&mUpperView, Lambd0, Lambd0, Lambd0, Lambd0, "Screenshot [I]"),
        gs::RectangleTextContainer(&mUpperView, Lambd0, Lambd0, Lambd0, Lambd0, "Export to csv [E]")},
    mPrintScreenButton(mExportButtons[0]),
    mToCsvButton(mExportButtons[1]),
    //EXPORT BUTTONS/

    //CHECKBOXES
    mCheckboxes{
        gs::Checkbox(gs::App::checkboxUnchecked, gs::App::checkboxChecked, &mUpperView,  true, Lambd0, [&] () {mPMS.rVecVisibilitySwitch();}, Lambd0, Lambd0),
        gs::Checkbox(gs::App::checkboxUnchecked, gs::App::checkboxChecked, &mUpperView,  true, Lambd0, [&] () {mPMS.xVecVisibilitySwitch();}, Lambd0, Lambd0),
        gs::Checkbox(gs::App::checkboxUnchecked, gs::App::checkboxChecked, &mUpperView,  true, Lambd0, [&] () {mPMS.yVecVisibilitySwitch();}, Lambd0, Lambd0),
        gs::Checkbox(gs::App::checkboxUnchecked, gs::App::checkboxChecked, &mUpperView,  true, Lambd0, [&] () {mPMS.curveVisibilitySwitch();}, Lambd0, Lambd0),
        gs::Checkbox(gs::App::checkboxUnchecked, gs::App::checkboxChecked, &mUpperView,  true, Lambd0, [&] () {mPMS.guidesVisibilitySwitch();}, Lambd0, Lambd0),
        gs::Checkbox(gs::App::checkboxUnchecked, gs::App::checkboxChecked, &mUpperView, false, Lambd0, [&] () {mPMS.switchScallingMethod();}, Lambd0, Lambd0)},
    mVectorRCheckbox(mCheckboxes[0]),
    mVectorXCheckbox(mCheckboxes[1]),
    mVectorYCheckbox(mCheckboxes[2]),
    mCurveCheckbox(mCheckboxes[3]),
    mCrosshairCheckbox(mCheckboxes[4]),
    mScallingMethodCheckbox(mCheckboxes[5]),
    //CHECKBOXES/

    //CHECKBOX LABELS
    mCheckboxLabels{
        sf::Text("Resultant vector", App::arialFont, mFontSize),
        sf::Text("X-vector", App::arialFont, mFontSize),
        sf::Text("Y-vector", App::arialFont, mFontSize),
        sf::Text("Curve", App::arialFont, mFontSize),
        sf::Text("Axis guides", App::arialFont, mFontSize),
        sf::Text("Unequal axes' scaling", App::arialFont, mFontSize)},
    mVectorRLabel(mCheckboxLabels[0]),
    mVectorXLabel(mCheckboxLabels[1]),
    mVectorYLabel(mCheckboxLabels[2]),
    mCurveLabel(mCheckboxLabels[3]),
    mCrosshairLabel(mCheckboxLabels[4]),
    mScallingMethodLabel(mCheckboxLabels[5]),
    //CHECKBOX LABELS/
    //~~~~~~~~~~~~UPPER VIEW~~~~~~~~~~~~/

    //~~~~~~~~~~~~UPPER VIEW~~~~~~~~~~~~
    //SIMULATION CONTROL BUTTONS
    mSimulationControlButtons{
        gs::RectangleTextContainer(&mLowerView, Lambd0, Lambd0, Lambd0, Lambd0, "Start [S]"),
        gs::RectangleTextContainer(&mLowerView, Lambd0, Lambd0, Lambd0, Lambd0, "Reset [R]"),
        gs::RectangleTextContainer(&mLowerView, Lambd0, Lambd0, Lambd0, Lambd0, "Pause [P]")},
    mStartButton(mSimulationControlButtons[0]),
    mRestartButton(mSimulationControlButtons[1]),
    mPauseButton(mSimulationControlButtons[2]),
    //SIMULATION CONTROL BUTTONS/

    //TEXTS
    mParametersTexts{
        sf::Text("", App::arialFont, mFontSizeForParamsText),
        sf::Text("", App::arialFont, mFontSizeForParamsText),
        sf::Text("", App::arialFont, mFontSizeForParamsText),
        sf::Text("", App::arialFont, mFontSizeForParamsText)},
    mConstResultsTextLeft(mParametersTexts[0]),
    mConstResultsTextRight(mParametersTexts[1]),
    mDynamicResultsText(mParametersTexts[2]),
    mComparativeResultsText(mParametersTexts[3]),
    //TEXTS/
    //~~~~~~~~~~~~UPPER VIEW~~~~~~~~~~~~/

    mListManager(
        [&] () {mListManagementButton.setString(L"\u25BA");},
        [&] () {mListManagementButton.setString(L"\u25BC");})
{
    this->addEvent(sf::Event::Resized, bind(&InterfacePMS::pollEventResized, ref(*this), placeholders::_1));

    this->prepareViews();
}


void gs::InterfacePMS::prepareViews()
{
    this->prepareViewsVieports();
    this->prepareViewsCompositions();
}

void gs::InterfacePMS::prepareViewsVieports()
{
    mUpperView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 0.15f));
    mLowerView.setViewport(sf::FloatRect(0.f, 0.85f, 1.f, 0.15f));
    mGlobalView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
}

void gs::InterfacePMS::prepareViewsCompositions()
{
    this->prepareGlobalViewComposition();
    this->prepareUpperViewComposition();
    this->prepareLowerViewComposition();
}

void gs::InterfacePMS::prepareGlobalViewComposition()
{
    this->prepareCelestialBodiesList();
    this->prepareListManagementButtons();
}

void gs::InterfacePMS::prepareCelestialBodiesList()
{
    sf::RectangleShape r(sf::Vector2f(150.f, 574.f));
    r.setFillColor(sf::Color::Green);
    r.setPosition(3.f, 105.f);

    array<sf::Sprite, gs::App::celestialBodyCount> cb;

    for(unsigned short i = 0; i < gs::App::celestialBodyCount; ++i)
    {
        cb[i].setTexture(gs::App::celestialBodies);
        cb[i].setTextureRect(sf::IntRect(i * gs::App::singleCelestialBodyDims.x, 0, gs::App::singleCelestialBodyDims.x, gs::App::singleCelestialBodyDims.y));
    }

    unsigned short const fontSize = 14;
    array<sf::Text, gs::App::celestialBodyCount> t = {
        sf::Text("Sun\n274.00 m/s^2", gs::App::arialFont, fontSize),
        sf::Text("Mercury\n3.70 m/s^2", gs::App::arialFont, fontSize),
        sf::Text("Venus\n8.87 m/s^2", gs::App::arialFont, fontSize),
        sf::Text("Earth\n9.81 m/s^2", gs::App::arialFont, fontSize),
        sf::Text("Moon\n2.38 m/s^2", gs::App::arialFont, fontSize),
        sf::Text("Mars\n3.71 m/s^2", gs::App::arialFont, fontSize),
        sf::Text("Jupiter\n24.79 m/s^2", gs::App::arialFont, fontSize),
        sf::Text("Saturn\n10.44 m/s^2", gs::App::arialFont, fontSize),
        sf::Text("Uranus\n8.87 m/s^2", gs::App::arialFont, fontSize),
        sf::Text("Neptune\n11.15 m/s^2", gs::App::arialFont, fontSize),
        sf::Text("Pluto\n0.62 m/s^2", gs::App::arialFont, fontSize)};

    mCelestialBodiesList = gs::Table<gs::App::celestialBodyCount>(r, cb, t, 50.f);
}

void gs::InterfacePMS::prepareUpperViewComposition()
{
    this->prepareInputs();
    this->prepareExportButtons();
    this->prepareCheckboxes();
}

void gs::InterfacePMS::prepareListManagementButtons()
{
    mListManagementButton.setFillColor(sf::Color::White);
    mListManagementButton.setInnerLineColor(sf::Color::Cyan);
    mListManagementButton.setPosition(sf::Vector2f(3.f, 77.f));
    mListManagementButton.setSize(sf::Vector2f(28.f, 28.f));
    mListManagementButton.setCharacterSize(mFontSize);

    mListManagementButton.setOnHoverFunc(bind(&gs::RectangleTextContainer::setInnerLineColor, &mListManagementButton, sf::Color::Magenta));
    mListManagementButton.setPreHoverFunc(bind(&gs::RectangleTextContainer::setInnerLineColor, &mListManagementButton, sf::Color::Cyan));
    mListManagementButton.setOnClickFunc([&] () {mListManager();});
}

void gs::InterfacePMS::prepareInputs()
{
    this->prepareMpfTypeInputs();
    this->prepareLongTypeInputs();
}

void gs::InterfacePMS::prepareMpfTypeInputs()
{
    float positionX = 33.f;
    float const inputSpaceBetween = 170.f;
    unsigned short i = 0;

    for(; i < min(mInputLabels.size(), min(mSimParamInputs.size(), static_cast<size_t>(3))); ++i)
    {
        mInputLabels[i].setPosition(sf::Vector2f(positionX, 0.f));
        mInputLabels[i].setFillColor(sf::Color::Black);

        mSimParamInputs[i].setPosition(sf::Vector2f(positionX, 23.f));
        mSimParamInputs[i].setSize(sf::Vector2f(160.f, 28.f));
        mSimParamInputs[i].setCharacterSize(21);

        positionX += inputSpaceBetween;
    }

    positionX = 33.f;

    for(; i < min(mInputLabels.size(), mSimParamInputs.size()); ++i)
    {
        mInputLabels[i].setPosition(sf::Vector2f(positionX, 54.f));
        mInputLabels[i].setFillColor(sf::Color::Black);

        mSimParamInputs[i].setPosition(sf::Vector2f(positionX, 77.f));
        mSimParamInputs[i].setSize(sf::Vector2f(160.f, 28.f));
        mSimParamInputs[i].setCharacterSize(21);

        positionX += inputSpaceBetween;
    }

    mTimeMultiplierInput.setCharacterSize(15);
    mTimeMultiplierInput.setSize(sf::Vector2f(178.f, 28.f));
    mSimulationTimeInput.setCharacterSize(18);
    mSimulationTimeInput.setSize(sf::Vector2f(178.f, 28.f));
}

void gs::InterfacePMS::prepareLongTypeInputs()
{
    //INTERVALS
    mIntervalsNumLabel.setPosition(sf::Vector2f(565.f, 0.f));
    mIntervalsNumLabel.setFillColor(sf::Color::Black);

    mIntervalsNumInput.setPosition(sf::Vector2f(565.f, 23.f));
    mIntervalsNumInput.setSize(sf::Vector2f(120.f, 28.f));
    mIntervalsNumInput.setCharacterSize(21);

    //PRECISION
    mPrecisionLabel.setPosition(sf::Vector2f(565.f, 54.f));
    mPrecisionLabel.setFillColor(sf::Color::Black);

    mPrecisionInput.setPosition(sf::Vector2f(565.f, 77.f));
    mPrecisionInput.setSize(sf::Vector2f(120.f, 28.f));
    mPrecisionInput.setCharacterSize(21);
}

void gs::InterfacePMS::prepareExportButtons()
{
    for(auto & button : mExportButtons)
    {
        button.setFillColor(sf::Color::White);
        button.setInnerLineColor(sf::Color::Cyan);
        button.setSize(sf::Vector2f(150.f, 40.f));
        button.setCharacterSize(mFontSize);

        button.setOnHoverFunc(bind(&gs::RectangleTextContainer::setInnerLineColor, &button, sf::Color::Magenta));
        button.setPreHoverFunc(bind(&gs::RectangleTextContainer::setInnerLineColor, &button, sf::Color::Cyan));
        button.setPreClickFunc(bind(&gs::RectangleTextContainer::setFillColor, &button, sf::Color::White));
    }

    mToCsvButton.setPosition(sf::Vector2f(700.f, 15.f));
    mPrintScreenButton.setPosition(sf::Vector2f(700.f, 65.f));

    mToCsvButton.setOnClickFunc(bind(&gs::ProjectileMotionSimulation::exportDataToCSVAsync, &mPMS));
    mPrintScreenButton.setOnClickFunc(bind(&gs::ProjectileMotionSimulation::exportViewToPNGAsync, &mPMS));
}

void gs::InterfacePMS::prepareCheckboxes()
{
    //CHECKBOXES - IMAGES  positionX
    float const checkboxStartX = 900.f;
    float const checkboxStartY = 15.f;
    float const checkboxSpacingY = 30.f;
    float const columnSpacingX = 180.f;

    unsigned short columnIndex = 0;
    unsigned short checkboxIndex = 0;

    for(auto &checkbox : mCheckboxes)
    {
        float const &&x = checkboxStartX + columnIndex * columnSpacingX;
        float const &&y = checkboxStartY + (checkboxIndex % 3) * checkboxSpacingY;

        checkbox.setPosition(sf::Vector2f(x, y));

        if((checkboxIndex + 1) % 3 == 0)
        {
            columnIndex++;
        }

        checkboxIndex++;
    }

    columnIndex = 0;
    checkboxIndex = 0;

    //CHECKBOXES - LABELS
    for(auto &checkboxLabel : mCheckboxLabels)
    {
        float const &&x = checkboxStartX + columnIndex * columnSpacingX + 35.f;     // checkbox's right-side offset
        float const &&y = checkboxStartY + (checkboxIndex % 3) * checkboxSpacingY;

        checkboxLabel.setPosition(sf::Vector2f(x, y));
        checkboxLabel.setFillColor(sf::Color::Black);

        if((checkboxIndex + 1) % 3 == 0)
        {
            ++columnIndex;
        }

        ++checkboxIndex;
    }
}

void gs::InterfacePMS::prepareLowerViewComposition()
{
    this->prepareSimulationControlButtons();
    this->prepareParametersTexts();
}

void gs::InterfacePMS::prepareSimulationControlButtons()
{
    for(auto & button : mSimulationControlButtons)
    {
        button.setFillColor(sf::Color::White);
        button.setInnerLineColor(sf::Color::Cyan);
        button.setSize(sf::Vector2f(100.f, 50.f));
        button.setCharacterSize(mSimulationControlButtonsFontSize);

        button.setOnHoverFunc(bind(&gs::RectangleTextContainer::setInnerLineColor, &button, sf::Color::Magenta));
        button.setPreHoverFunc(bind(&gs::RectangleTextContainer::setInnerLineColor, &button, sf::Color::Cyan));
        button.setPreClickFunc(bind(&gs::RectangleTextContainer::setFillColor, &button, sf::Color::White));
    }

    mStartButton.setPosition(sf::Vector2f(33.f, 10.f));
    mRestartButton.setPosition(mStartButton.getPosition() + sf::Vector2f(mStartButton.getSize().x + 8.f, 0.f));
    mPauseButton.setPosition(mRestartButton.getPosition() + sf::Vector2f(mRestartButton.getSize().x + 8.f, 0.f));

    mStartButton.setOnClickFunc(bind(&gs::ProjectileMotionSimulation::start, &mPMS));
    mRestartButton.setOnClickFunc(bind(&gs::ProjectileMotionSimulation::reset, &mPMS));
    mPauseButton.setOnClickFunc(bind(&gs::ProjectileMotionSimulation::pause, &mPMS));
}

void gs::InterfacePMS::prepareParametersTexts()
{
    array<float const, 2> const columnText_xPos = {
        mPauseButton.getPosition().x + mPauseButton.getSize().x + 10.f,
        670.f};

    for(sf::Text &text : mParametersTexts)
    {
        text.setFillColor(sf::Color::Black);
    }

    mConstResultsTextLeft.setPosition(columnText_xPos[0], 0.f);
    mDynamicResultsText.setPosition(columnText_xPos[0], 34.f);

    mConstResultsTextRight.setPosition(columnText_xPos[1], 0.f);
    mComparativeResultsText.setPosition(columnText_xPos[1], 70.f);
}


void gs::InterfacePMS::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.setView(mUpperView);
        this->drawOnUpperView(target, states);

    target.setView(mLowerView);
        this->drawOnLowerView(target, states);

    target.setView(mGlobalView);
        this->drawOnGlobalView(target, states);
}

void gs::InterfacePMS::drawOnUpperView(sf::RenderTarget &target, sf::RenderStates states) const
{
    for(sf::Text const &label : mInputLabels)
    {
        target.draw(label, states);
    }

    for(gs::RectangleInputFieldContainer<mpf_class> const &input : mSimParamInputs)
    {
        target.draw(input, states);
    }

    target.draw(mIntervalsNumInput, states);
    target.draw(mPrecisionInput, states);

    for(gs::RectangleTextContainer const &button : mExportButtons)
    {
        target.draw(button, states);
    }

    for(sf::Text const &label : mCheckboxLabels)
    {
        target.draw(label, states);
    }

    for(gs::Checkbox const &checkbox : mCheckboxes)
    {
        target.draw(checkbox, states);
    }
}

void gs::InterfacePMS::drawOnLowerView(sf::RenderTarget &target, sf::RenderStates states) const
{
    for(gs::RectangleTextContainer const &button : mSimulationControlButtons)
    {
        target.draw(button, states);
    }

    for(sf::Text const &text : mParametersTexts)
    {
        target.draw(text, states);
    }
}

void gs::InterfacePMS::drawOnGlobalView(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mListManagementButton, states);

    if(mListManager.state)
    {
        target.draw(mCelestialBodiesList, states);
    }
}


void gs::InterfacePMS::pollEventResized(sf::Event const &event)
{
    mUpperView.reset(sf::FloatRect(0.f, 0.f, 1280, 108));
    mLowerView.reset(sf::FloatRect(0.f, 0.f, 1280, 108));
    mGlobalView.reset(sf::FloatRect(0.f, 0.f, 1280, 720));
}


gs::RectangleTextContainer & gs::InterfacePMS::getToCsvButton()
{
    return mToCsvButton;
}


void gs::InterfacePMS::update()
{
    this->updateUpperView();
    this->updateLowerView();
    this->updateGlobalView();
}

void gs::InterfacePMS::updateUpperView()
{
    this->updateInputs();
    this->updateExportButtons();
    this->updateCheckboxes();
}

void gs::InterfacePMS::updateLowerView()
{
    this->updateSimulationControlButtons();
    this->updateParametersTexts();
}

void gs::InterfacePMS::updateGlobalView()
{
    mListManagementButton.update();

    if(mListManager.state)
    {
        short const &n = mCelestialBodiesList.update(mGlobalView);

        if(n > - 1 && n < gs::App::celestialBodyCount)
        {
            mAccInput.setInput(mAccArr[mCelestialBodiesList.update(mGlobalView)]);
            mPMS.setAcc(mAccInput.getValue());
            mListManager();
        }
    }
}

void gs::InterfacePMS::updateInputs()
{
    array<reference_wrapper<gs::RectangleInputFieldContainer<mpf_class>>, 5> selectedInputs = {
        mAngleInput, mHeightInput, mTimeMultiplierInput, mAccInput, mVelocityInput};

    for(size_t i = 0; i < selectedInputs.size(); ++i)
    {
        auto &input = selectedInputs[i].get();

        input.update();

        if(input.isCurrentValueNew())
        {
            (mPMS.*mMpfPMS_methods[i])(input.getValue());
        }
    }

    mSimulationTimeInput.update();
    if(mSimulationTimeInput.isValueApproved())
    {
        mPMS.setSimulationAtGiventTime(mSimulationTimeInput.getValue());
    }

    for(auto &input : mExportSettingsInput)
    {
        input.update();
    }

    if(mIntervalsNumInput.isCurrentValueNew())
    {
        mPMS.setIntervalsCount(static_cast<unsigned short>(mIntervalsNumInput.getValue()));
    }

    if(mPrecisionInput.isCurrentValueNew())
    {
        mPMS.setExportedDataPrecision(static_cast<short>(mPrecisionInput.getValue()));
    }
}

void gs::InterfacePMS::updateExportButtons()
{
    for(auto &button : mExportButtons)
    {
        button.update();
    }
}

void gs::InterfacePMS::updateSimulationControlButtons()
{
    for(auto &button : mSimulationControlButtons)
    {
        button.update();
    }
}


void gs::InterfacePMS::updateCheckboxes()
{
    for(auto &checkbox : mCheckboxes)
    {
        checkbox.update();
    }
}

void gs::InterfacePMS::updateParametersTexts()
{
    gs::ProjectileMotionObject const &pmo = mPMS.getPMO();

    this->updateConstResultsTextLeft(pmo);
    this->updateConstResultsTextRight(pmo);
    this->updateDynamicResultsText(pmo);
    this->updateComparativeResultsText(pmo);
}

void gs::InterfacePMS::updateConstResultsTextLeft(gs::ProjectileMotionObject const &pmo)
{
    ostringstream text;

    this->outputMpfValueToStream(text, strFormat_Vx, pmo.getVx());
    this->outputMpfValueToStream(text, strFormat_Vy0, pmo.getVy0());

    mConstResultsTextLeft.setString(text.str());
}

void gs::InterfacePMS::updateConstResultsTextRight(gs::ProjectileMotionObject const &pmo)
{
    ostringstream text;

    this->outputMpfValueToStream(text, strFormat_rT, pmo.getRaiseTime());
    this->outputMpfValueToStream(text, strFormat_dT, pmo.getDescentTime());
    this->outputMpfValueToStream(text, strFormat_mH, pmo.getMaxH());

    mConstResultsTextRight.setString(text.str());
}

void gs::InterfacePMS::updateDynamicResultsText(gs::ProjectileMotionObject const &pmo)
{
    ostringstream text;

    this->outputMpfValueToStream(text, strFormat_Vy, pmo.getCurrentVy());
    this->outputMpfValueToStream(text, strFormat_V, pmo.getCurrentV());
    this->outputMpfValueToStream(text, strFormat_a, gs::toDegrees(pmo.getCurrentAngle()));
    this->outputMpfValueToStream(text, strFormat_h, pmo.getCurrentH());

    mDynamicResultsText.setString(text.str());
}

void gs::InterfacePMS::updateComparativeResultsText(gs::ProjectileMotionObject const &pmo)
{
    ostringstream text;

    this->outputMpfValueToStream(text, strFormat_d, pmo.getCurrentDistance());
    this->outputMpfValueToStream(text, strFormat_r, pmo.getReach());
    this->outputMpfValueToStream(text, strFormat_t, pmo.getElapsedTime_s());
    this->outputMpfValueToStream(text, strFormat_fT, pmo.getFlightTime());

    mComparativeResultsText.setString(text.str());
}


void gs::InterfacePMS::outputMpfValueToStream(ostringstream &o, std::pair<char const * const, char const * const> const &numValueOutput, mpf_class const &mpfValue)
{
    char *buffer = nullptr;

    gmp_asprintf(&buffer, mStrValueFormat, mpfValue);
    o << numValueOutput.first << gs::getNumWithThousandsSeparator(buffer) << numValueOutput.second;
    free(buffer);
}

