#ifndef INTERFACEPMS_H
#define INTERFACEPMS_H

#include "Checkbox.h"
#include "EventsHandler.h"
#include "Mouse.h"
#include "ProjectileMotionObject.h"
#include "RectangleInputFieldContainer.h"
#include "RectangleTextContainer.h"
#include "Table.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <set>
#include <vector>


namespace gs
{

class ProjectileMotionSimulation;

class InterfacePMS final: public gs::EventsHandler, public sf::Drawable
{
private:
    static constexpr char const * const mStrValueFormat = "%.2Ff";

    using cstrPair_t = std::pair<char const * const, char const * const>;
    static constexpr cstrPair_t strFormat_Vx  = {"       Vx:    ", " m/s\n"};
    static constexpr cstrPair_t strFormat_Vy0 = {"     Vy0:    ", " m/s\n"};
    static constexpr cstrPair_t strFormat_Vy  = {"       Vy:    ", " m/s\n"};
    static constexpr cstrPair_t strFormat_V   = {"         V:    ", " m/s\n"};
    static constexpr cstrPair_t strFormat_a   = {"  Angle:    ", "*\n"};
    static constexpr cstrPair_t strFormat_h   = {"Height:    ", " m\n"};
    static constexpr cstrPair_t strFormat_rT  = {"    Raise time:    ", " s\n"};
    static constexpr cstrPair_t strFormat_dT  = {"Descent time:    ", " s\n"};
    static constexpr cstrPair_t strFormat_mH  = {"    Max height:    ", " m\n"};
    static constexpr cstrPair_t strFormat_d   = {"        Distance:    ", " / "};
    static constexpr cstrPair_t strFormat_r   = {"", " [m]\n"};
    static constexpr cstrPair_t strFormat_t   = {"               Time:    ", " / "};
    static constexpr cstrPair_t strFormat_fT  = {"", " [s]\n"};

    static constexpr unsigned short mFontSize = 17;
    static constexpr unsigned short mFontSizeForParamsText = 16;

    static inline std::array<std::string, gs::App::celestialBodyCount> const mAccArr = {
        "274.00",
        "3.70",
        "8.87",
        "9.81",
        "2.38",
        "3.71",
        "24.79",
        "10.44",
        "8.87",
        "11.15",
        "0.62"};

    gs::ProjectileMotionSimulation &mPMS;

    sf::View mUpperView, mLowerView, mGlobalView;

    static constexpr unsigned short mSimParamInputsCount = 6;
    static constexpr unsigned short mExportSettingsInputCount = 2;

    std::array<void (ProjectileMotionSimulation::* const)(mpf_class const &), mSimParamInputsCount> const mMpfPMS_methods;

    //~~~~~~~~~~~~GLOBAL VIEW~~~~~~~~~~~~
    gs::Table<11> mCelestialBodiesList;

    gs::RectangleTextContainer mListManagementButton;
    //~~~~~~~~~~~~GLOBAL VIEW~~~~~~~~~~~~/

    gs::BinarySwitch mListManager;


    //~~~~~~~~~~~~UPPER VIEW~~~~~~~~~~~~
    //INPUT FIELDS
    std::array<gs::RectangleInputFieldContainer<mpf_class>, mSimParamInputsCount> mSimParamInputs;
        gs::RectangleInputFieldContainer<mpf_class> &mAngleInput;
        gs::RectangleInputFieldContainer<mpf_class> &mHeightInput;
        gs::RectangleInputFieldContainer<mpf_class> &mTimeMultiplierInput;
        gs::RectangleInputFieldContainer<mpf_class> &mAccInput;
        gs::RectangleInputFieldContainer<mpf_class> &mVelocityInput;
        gs::RectangleInputFieldContainer<mpf_class> &mSimulationTimeInput;

    std::array<gs::RectangleInputFieldContainer<long>, mExportSettingsInputCount> mExportSettingsInput;
        gs::RectangleInputFieldContainer<long> &mIntervalsNumInput;
        gs::RectangleInputFieldContainer<long> &mPrecisionInput;
    //INPUT FIELDS/

    //INPUT FIELD LABELS
    std::array<sf::Text, mSimParamInputsCount + mExportSettingsInputCount> mInputLabels;
        sf::Text &mAngleLabel;
        sf::Text &mHeightLabel;
        sf::Text &mTimeMultiplierLabel;
        sf::Text &mSimulationTimeLabel;
        sf::Text &mVelocityLabel;
        sf::Text &mAccLabel;
        sf::Text &mIntervalsNumLabel;
        sf::Text &mPrecisionLabel;
    //INPUT FIELD LABELS/

    //EXPORT BUTTONS
    std::array<gs::RectangleTextContainer, 2>  mExportButtons;
        gs::RectangleTextContainer &mPrintScreenButton;
        gs::RectangleTextContainer &mToCsvButton;
    //EXPORT BUTTONS

    //CHECKBOXES
    std::array<gs::Checkbox, 6> mCheckboxes;
        gs::Checkbox &mVectorRCheckbox;
        gs::Checkbox &mVectorXCheckbox;
        gs::Checkbox &mVectorYCheckbox;
        gs::Checkbox &mCurveCheckbox;
        gs::Checkbox &mCrosshairCheckbox;
        gs::Checkbox &mScallingMethodCheckbox;
    //CHECKBOXES/

    //CHECKBOX LABELS
    std::array<sf::Text, 6> mCheckboxLabels;
        sf::Text &mVectorRLabel;
        sf::Text &mVectorXLabel;
        sf::Text &mVectorYLabel;
        sf::Text &mCurveLabel;
        sf::Text &mCrosshairLabel;
        sf::Text &mScallingMethodLabel;
    //CHECKBOX LABELS/
    //~~~~~~~~~~~~UPPER VIEW~~~~~~~~~~~~/


    //~~~~~~~~~~~~LOWER VIEW~~~~~~~~~~~~
    //SIMULATION CONTROL BUTTONS
    static constexpr unsigned short mSimulationControlButtonsFontSize = 19;
    std::array<gs::RectangleTextContainer, 3> mSimulationControlButtons;
        gs::RectangleTextContainer &mStartButton;
        gs::RectangleTextContainer &mRestartButton;
        gs::RectangleTextContainer &mPauseButton;
    //SIMULATION CONTROL BUTTONS/

    //TEXTS
    std::array<sf::Text, 4> mParametersTexts;
        sf::Text &mConstResultsTextLeft;
        sf::Text &mConstResultsTextRight;
        sf::Text &mDynamicResultsText;
        sf::Text &mComparativeResultsText;
    //TEXTS/
    //~~~~~~~~~~~~LOWER VIEW~~~~~~~~~~~~/


public:
    explicit InterfacePMS(gs::ProjectileMotionSimulation &pms);


private:
    void prepareViews();
        void prepareViewsVieports();
        void prepareViewsCompositions();
            void prepareGlobalViewComposition();
                void prepareCelestialBodiesList();
            void prepareUpperViewComposition();
                void prepareListManagementButtons();
                void prepareInputs();
                    void prepareMpfTypeInputs();
                    void prepareLongTypeInputs();
                void prepareExportButtons();
                void prepareCheckboxes();
            void prepareLowerViewComposition();
                void prepareSimulationControlButtons();
                void prepareParametersTexts();

    //from sf::Drawable
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    //from sf::Drawable/
        void drawOnUpperView(sf::RenderTarget &target, sf::RenderStates states) const;
        void drawOnLowerView(sf::RenderTarget &target, sf::RenderStates states) const;
        void drawOnGlobalView(sf::RenderTarget &target, sf::RenderStates states) const;

    void pollEventResized(sf::Event const &event);

public:
    gs::RectangleTextContainer & getToCsvButton();
    void update();

private:
        void updateUpperView();
            void updateInputs();
            void updateExportButtons();
            void updateCheckboxes();
        void updateLowerView();
            void updateSimulationControlButtons();
            void updateParametersTexts();
                void updateConstResultsTextLeft(gs::ProjectileMotionObject const &pmo);
                void updateConstResultsTextRight(gs::ProjectileMotionObject const &pmo);
                void updateDynamicResultsText(gs::ProjectileMotionObject const &pmo);
                void updateComparativeResultsText(gs::ProjectileMotionObject const &pmo);
        void updateGlobalView();

    void outputMpfValueToStream(ostringstream &o, std::pair<char const * const, char const * const> const &numValueOutput, mpf_class const &mpfValue);
};

}

#endif // INTERFACEPMS_H
