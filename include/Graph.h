#ifndef GRAPH_H
#define GRAPH_H

#include "BoundingRect.h"
#include "Vector.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <array>
#include <fstream>
#include <utility>
#include <vector>


namespace gs
{

class Graph final
{
public:
    static constexpr gs::BoundingRect<float> mGraphMargin = gs::BoundingRect<float>(32.f, 63.f, 32.f, 26.f);

private:
    static constexpr std::array<short, 5> mUnits = {1, 2, 3, 4, 5};
    static constexpr std::pair<short, short> mExpOf10Range = {-8, 19};

    static constexpr unsigned short mGraphValueSize = 16;
    static constexpr unsigned short graphKeyFontSize = 15;

    static constexpr std::array<const char * const, 4> mGradNames = {"xLGrad.png", "xRGrad.png", "yBGrad.png", "yTGrad.png"};
    static constexpr char const * const mMaxWindowDimsFileName = "maxWindowDims.txt";

    static constexpr unsigned short mInterval = 50;
    static constexpr unsigned short mGradJump = 2;

    sf::Vector2<short> mCurrentUnit;
    sf::Vector2<short> mCurrentExpOf10;
    sf::Vector2<mpf_class> mScale;
    void (Graph::* mScalingMethodPtr)(sf::Vector2<mpf_class> const &, sf::View const &);

    sf::Vector2<gs::Vector> mAxes;
    std::array<sf::Texture, 4> mGradTextures;

    std::array<sf::Sprite, 4> mGrads;
        sf::Sprite &mLGrad;
        sf::Sprite &mRGrad;
        sf::Sprite &mBGrad;
        sf::Sprite &mTGrad;

    sf::Vector2<sf::Text> mGraphKeys;
    std::array<mpf_class, mExpOf10Range.first < 0 ? static_cast<size_t>(-mExpOf10Range.first) * mUnits.size() : 0> rationalScales;
    std::array<mpz_class, mExpOf10Range.second >= 0 ? static_cast<size_t>(mExpOf10Range.second + 1) * mUnits.size() : 0> integerScales;

    std::array<std::vector<sf::Text>, 3> mValues;
        std::vector<sf::Text> &mRValues;
        std::vector<sf::Text> &mBValues;
        std::vector<sf::Text> &mTValues;

    short mLastVisibleRValueIdx; //-1 - no value is visible
    short mLastVisibleBValueIdx; //-1 - no value is visible
    short mLastVisibleTValueIdx; //-1 - no value is visible


public:
    Graph(sf::View const &view);


private:
    void lineGraduationUp(sf::Color const gradColor = sf::Color::Black);
        void createGraduation(sf::Color const gradColor);
            void createXGraduation(sf::Color const gradColor, sf::RenderTexture &rt, sf::Image &im);
            void createYGraduation(sf::Color const gradColor, sf::RenderTexture &rt, sf::Image &im);
    void lineArraysOfScaleUp();
        void lineRationalScaleArrayUp();
        void lineIntegerScaleArrayUp();
    void lineGraphKeysUp(sf::Vector2f const &viewCenter, sf::Vector2f const &viewHalfSize);
    void lineGradsUp();
    void lineValuesUp();


public:
    unsigned short getInterval() const;
    sf::Vector2<mpf_class> const & getScale() const;
    sf::Vector2<gs::Vector> const & getAxes() const;

    std::array<sf::Sprite, 4> const & getGrads() const;
        sf::Sprite const & getLGrad() const;
        sf::Sprite const & getRGrad() const;
        sf::Sprite const & getBGrad() const;
        sf::Sprite const & getTGrad() const;

    sf::Vector2<sf::Text> const & getGraphKeys() const;

    std::array<std::vector<sf::Text>, 3> const & getValues() const;
        std::vector<sf::Text> const & getRValues() const;
        std::vector<sf::Text> const & getBValues() const;
        std::vector<sf::Text> const & getTValues() const;

    short getLastVisibleRValueIdx() const;
    short getLastVisibleBValueIdx() const;
    short getLastVisibleTValueIdx() const;

    void setXAxisPointOfApp(sf::Vector2f const &point);
    void setYAxisPointOfApp(sf::Vector2f const &point);

    void setXKeyPosition(sf::Vector2f const &position);
    void setYKeyPosition(sf::Vector2f const &position);

    void switchScalingMethod();

    void updateAtResize(sf::View &view);
    void updateAtViewMove(sf::View const &view);
    void updateScale(sf::Vector2<mpf_class> const &maxValuesInDims, sf::View const &view);


private:
    void updateSymmetricScale(sf::Vector2<mpf_class> const &maxValuesInDims, sf::View const &view);
    void updateAsymmetricScale(sf::Vector2<mpf_class> const &maxValuesInDims, sf::View const &view);
    void calcScaleX(mpf_class const &maxHorizontalValue, sf::View const &view);
    void calcScaleY(mpf_class const &maxVerticalValue, sf::View const &view);
    std::pair<short, short> findFitScale(std::pair<mpf_class, mpf_class> const &valueAndTicks) const;
    mpf_class calcScale(std::pair<short, short> const &unitAndExpOf10);
    void setNewScale(std::pair<sf::Vector2<short>, sf::Vector2<short>> const &unitAndExpOf10Pair, sf::Vector2<mpf_class> const &newScale);
    void setNewScaleX(std::pair<short, short> const &unitAndExpOf10X, mpf_class const &newScaleX);
    void setNewScaleY(std::pair<short, short> const &unitAndExpOf10Y, mpf_class const &newScaleY);

    void setLastVisibleValuesIndexes(sf::Vector2f const &viewCenter, sf::Vector2f const &viewHalfSize);
        void setLastVisibleRValuesIndex(sf::Vector2f const &viewCenter, sf::Vector2f const &viewHalfSize);
        void setLastVisibleBValuesIndex(sf::Vector2f const &viewCenter, sf::Vector2f const &viewHalfSize);
        void setLastVisibleTValuesIndex(sf::Vector2f const &viewCenter, sf::Vector2f const &viewHalfSize);

    void setAllGradValues(unsigned short const jump = 1);
        void setXGradValues(unsigned short const jump = 1);
        void setYGradValues(unsigned short const jump = 1);
};

}

#endif // GRAPH_H
