#include "Graph.h"

#include "App.h"
#include "StringFunctions.h"

#include <SFML/Graphics/RenderTexture.hpp>

#include <cassert>
#include <filesystem>


using namespace std;


gs::Graph::Graph(sf::View const &view):
    mCurrentUnit(mUnits[0], mUnits[0]),
    mCurrentExpOf10(mExpOf10Range.first, mExpOf10Range.first),
    mScale(mpf_class(mInterval) / mpf_class(mCurrentUnit.x) / mpf_class(gs::makeNumberStr(1, mCurrentExpOf10.x, "", 0, 0, ".")),
           mpf_class(mInterval) / mpf_class(mCurrentUnit.y) / mpf_class(gs::makeNumberStr(1, mCurrentExpOf10.y, "", 0, 0, "."))),
    mScalingMethodPtr(&updateSymmetricScale),

    mAxes{gs::Vector(view.getCenter() - sf::Vector2f(view.getSize().x / 2.f, -(view.getSize().y / 2.f - mGraphMargin.bottom)),
                     view.getSize().x, 1, 0.f, 3, sf::Color::Black, 30, false, sf::Color::Black),
          gs::Vector(sf::Vector2f(view.getCenter().x - view.getSize().x / 2.f + mGraphMargin.left, view.getCenter().y + view.getSize().y / 2.f),
                     view.getSize().y, 1, 90.f, 3, sf::Color::Black, 30, false, sf::Color::Black)},
    mGradTextures{},

    mGrads{},
    mLGrad(mGrads[0]),
    mRGrad(mGrads[1]),
    mBGrad(mGrads[2]),
    mTGrad(mGrads[3]),

    mGraphKeys{
        sf::Text("[m]", gs::App::arialFont, graphKeyFontSize),
        sf::Text("[m]", gs::App::arialFont, graphKeyFontSize)},
    rationalScales{2},
    integerScales{2},

    mValues{
        vector<sf::Text>(gs::App::maxWindowDims.x / 102, sf::Text("", gs::App::arialFont, mGraphValueSize)),
        vector<sf::Text>(gs::App::maxWindowDims.y / 102, sf::Text("", gs::App::arialFont, mGraphValueSize)),
        vector<sf::Text>(gs::App::maxWindowDims.y / 102, sf::Text("", gs::App::arialFont, mGraphValueSize))},
    mRValues(mValues[0]),
    mBValues(mValues[1]),
    mTValues(mValues[2]),
    mLastVisibleRValueIdx(-1),
    mLastVisibleBValueIdx(-1),
    mLastVisibleTValueIdx(-1)
{
    sf::Vector2f const viewCenter(view.getCenter()), viewHalfSize(view.getSize() / 2.f);

    this->lineGraduationUp();
    this->lineArraysOfScaleUp();
    this->lineGraphKeysUp(viewCenter, viewHalfSize);
    this->lineGradsUp();
    this->lineValuesUp();

    this->setAllGradValues(mGradJump);
    this->setLastVisibleValuesIndexes(viewCenter, viewHalfSize);
}


void gs::Graph::lineGraduationUp(sf::Color const gradColor)
{
    auto const saveTextToFile =
        [] (char const * const &fileName, string const &text) {if(ofstream file(gs::App::assetsDir / fileName); file.is_open()) {file << text; file.close();}};

    if(   any_of(mGradNames.cbegin(), mGradNames.cend(), [] (char const * const fileName) {return !filesystem::exists(gs::App::assetsDir / fileName);})
       || !filesystem::exists(gs::App::assetsDir / mMaxWindowDimsFileName))
    {
        saveTextToFile(mMaxWindowDimsFileName, to_string(gs::App::maxWindowDims.x) + '\n' + to_string(gs::App::maxWindowDims.y));

        this->createGraduation(gradColor);
    }
    else if(ifstream inputFile(gs::App::assetsDir / mMaxWindowDimsFileName); inputFile.is_open())
    {
        string lines[2];

        getline(inputFile, lines[0]);
        getline(inputFile, lines[1]);

        if(stoul(lines[0]) != gs::App::maxWindowDims.x || stoul(lines[1]) != gs::App::maxWindowDims.y)
        {
            saveTextToFile(mMaxWindowDimsFileName, to_string(gs::App::maxWindowDims.x) + '\n' + to_string(gs::App::maxWindowDims.y));
            this->createGraduation(gradColor);
        }

        inputFile.close();
    }
    else
    {
        cerr << "Error: Unable to open " << mMaxWindowDimsFileName << endl;
    }
}

void gs::Graph::createGraduation(sf::Color const gradColor)
{
    sf::RenderTexture renderTexture;
    sf::Image image;

    this->createXGraduation(gradColor, renderTexture, image);
    this->createYGraduation(gradColor, renderTexture, image);
}

void gs::Graph::createXGraduation(sf::Color const gradColor, sf::RenderTexture &rt, sf::Image &im)
{
    sf::VertexArray graduation(sf::PrimitiveType::Quads, gs::App::maxWindowDims.x / 51 * 4);
    sf::Vector2f pos = {};

    for(unsigned short i = 0; i < graduation.getVertexCount();)
    {
        graduation[i++] = sf::Vertex(pos, gradColor);
        graduation[i++] = sf::Vertex(pos + sf::Vector2f(0.f, 10.f), gradColor);
        graduation[i++] = sf::Vertex(pos + sf::Vector2f(3.f, 10.f), gradColor);
        graduation[i++] = sf::Vertex(pos + sf::Vector2f(3.f,  0.f), gradColor);

        pos.x += static_cast<float>(mInterval);
    }

    rt.create(gs::App::maxWindowDims.x, 10);
    rt.clear(sf::Color::Transparent);
    rt.draw(graduation);
    rt.display();

    im = rt.getTexture().copyToImage();
    im.saveToFile((gs::App::assetsDir / mGradNames[1]).string());
    im.flipHorizontally();
    im.saveToFile((gs::App::assetsDir / mGradNames[0]).string());
}

void gs::Graph::createYGraduation(sf::Color const gradColor, sf::RenderTexture &rt, sf::Image &im)
{
    sf::VertexArray graduation(sf::PrimitiveType::Quads, gs::App::maxWindowDims.y / 51 * 4);
    sf::Vector2f pos = {};

    for(unsigned short i = 0; i < graduation.getVertexCount();)
    {
        graduation[i++] = sf::Vertex(pos, gradColor);
        graduation[i++] = sf::Vertex(pos + sf::Vector2f(0.f, 3.f), gradColor);
        graduation[i++] = sf::Vertex(pos + sf::Vector2f(10.f, 3.f), gradColor);
        graduation[i++] = sf::Vertex(pos + sf::Vector2f(10.f, 0.f), gradColor);

        pos.y += static_cast<float>(mInterval);
    }

    rt.create(10, gs::App::maxWindowDims.y);
    rt.clear(sf::Color::Transparent);
    rt.draw(graduation);
    rt.display();

    im = rt.getTexture().copyToImage();
    im.saveToFile((gs::App::assetsDir / mGradNames[2]).string());
    im.flipVertically();
    im.saveToFile((gs::App::assetsDir / mGradNames[3]).string());
}

void gs::Graph::lineArraysOfScaleUp()
{
    if(!rationalScales.empty())
    {
        this->lineRationalScaleArrayUp();
    }

    if(!integerScales.empty())
    {
        this->lineIntegerScaleArrayUp();
    }
}

void gs::Graph::lineRationalScaleArrayUp()
{
    for(unsigned short i = -mExpOf10Range.first, idx = 0; i >= -min(mExpOf10Range.second, static_cast<short>(-1)); --i)
    {
        for(unsigned short const u : mUnits)
        {
            mpf_class powerOf10(0);

            mpf_pow_ui(powerOf10.get_mpf_t(), mpf_class("10.00").get_mpf_t(), i);
            rationalScales[idx++] = mpf_class(u) / powerOf10;
        }
    }
}

void gs::Graph::lineIntegerScaleArrayUp()
{
    for(unsigned short i = 0, idx = 0; i <= mExpOf10Range.second; ++i)
    {
        for(unsigned short const u : mUnits)
        {
            mpz_class powerOf10(0);

            mpz_pow_ui(powerOf10.get_mpz_t(), mpz_class(10).get_mpz_t(), i);
            integerScales[idx++] = powerOf10 * mpz_class(u);
        }
    }
}

void gs::Graph::lineGraphKeysUp(sf::Vector2f const &viewCenter, sf::Vector2f const &viewHalfSize)
{
    mGraphKeys.x.setFillColor(sf::Color::Black);
    mGraphKeys.x.setPosition(sf::Vector2f(viewCenter.x + viewHalfSize.x - 23.f, mAxes.x.getPointOfApp().y + 5.f));

    mGraphKeys.y.setOrigin(mGraphKeys.y.getOrigin() + sf::Vector2f(mGraphKeys.y.getLocalBounds().width, 0.f));
    mGraphKeys.y.setFillColor(sf::Color::Black);
    mGraphKeys.y.setPosition(sf::Vector2f(mAxes.y.getPointOfApp().x - 8.f, viewCenter.y - viewHalfSize.y));
}

void gs::Graph::lineGradsUp()
{
    for(unsigned short i = 0; i < mGrads.size(); ++i)
    {
        mGradTextures[i].loadFromFile((gs::App::assetsDir / mGradNames[i]).string());
        mGrads[i].setTexture(mGradTextures[i]);
    }

    mLGrad.setOrigin(mLGrad.getOrigin() + sf::Vector2f(static_cast<float>(mGradTextures[0].getSize().x), 5.f));
    mLGrad.setPosition(mAxes.y.getPointOfApp().x - 49.f, mAxes.x.getPointOfApp().y);

    mRGrad.setOrigin(mRGrad.getOrigin() + sf::Vector2f(0.f, 5.f));
    mRGrad.setPosition(mAxes.y.getPointOfApp().x + 49.f, mAxes.x.getPointOfApp().y);

    mBGrad.setOrigin(mBGrad.getOrigin() + sf::Vector2f(5.f, 0.f));
    mBGrad.setPosition(mAxes.y.getPointOfApp().x, mAxes.x.getPointOfApp().y + 49.f);

    mTGrad.setOrigin(mTGrad.getOrigin() + sf::Vector2f(5.f, static_cast<float>(mGradTextures[3].getSize().y)));
    mTGrad.setPosition(mAxes.y.getPointOfApp().x, mAxes.x.getPointOfApp().y - 49.f);
}

void gs::Graph::lineValuesUp()
{
    for(vector<sf::Text> &textVector : mValues)
    {
        for(sf::Text &text : textVector)
        {
            text.setFillColor(sf::Color::Black);
        }
    }
}


unsigned short gs::Graph::getInterval() const
{
    return mInterval;
}

sf::Vector2<mpf_class> const & gs::Graph::getScale() const
{
    return mScale;
}

sf::Vector2<gs::Vector> const & gs::Graph::getAxes() const
{
    return mAxes;
}


std::array<sf::Sprite, 4> const & gs::Graph::getGrads() const
{
    return mGrads;
}

sf::Sprite const & gs::Graph::getLGrad() const
{
    return mLGrad;
}

sf::Sprite const & gs::Graph::getRGrad() const
{
    return mRGrad;
}

sf::Sprite const & gs::Graph::getBGrad() const
{
    return mBGrad;
}

sf::Sprite const & gs::Graph::getTGrad() const
{
    return mTGrad;
}


sf::Vector2<sf::Text> const & gs::Graph::getGraphKeys() const
{
    return mGraphKeys;
}


std::array<std::vector<sf::Text>, 3> const & gs::Graph::getValues() const
{
    return mValues;
}

std::vector<sf::Text> const & gs::Graph::getRValues() const
{
    return mRValues;
}

std::vector<sf::Text> const & gs::Graph::getBValues() const
{
    return mBValues;
}

std::vector<sf::Text> const & gs::Graph::getTValues() const
{
    return mTValues;
}


short gs::Graph::getLastVisibleRValueIdx() const
{
    return mLastVisibleRValueIdx;
}

short gs::Graph::getLastVisibleBValueIdx() const
{
    return mLastVisibleBValueIdx;
}

short gs::Graph::getLastVisibleTValueIdx() const
{
    return mLastVisibleTValueIdx;
}


void gs::Graph::setXAxisPointOfApp(sf::Vector2f const &point)
{
    mAxes.x.setPointOfApp(point);
}

void gs::Graph::setYAxisPointOfApp(sf::Vector2f const &point)
{
    mAxes.y.setPointOfApp(point);
}


void gs::Graph::setXKeyPosition(sf::Vector2f const &position)
{
    mGraphKeys.y.setPosition(position);
}

void gs::Graph::setYKeyPosition(sf::Vector2f const &position)
{
    mGraphKeys.y.setPosition(position);
}


void gs::Graph::switchScalingMethod()
{
    if(mScalingMethodPtr == &updateSymmetricScale)
    {
        mScalingMethodPtr = &updateAsymmetricScale;
    }
    else
    {
        mScalingMethodPtr = &updateSymmetricScale;
    }
}


void gs::Graph::updateAtResize(sf::View &view)
{
    sf::Vector2f const viewHalfSize(view.getSize() / 2.f);

    mAxes.x.setUnits(view.getSize().x);
    mAxes.y.setUnits(view.getSize().y);

    if(mAxes.y.getPointOfApp().x - mGraphMargin.left != view.getCenter().x - viewHalfSize.x)
    {
        view.setCenter(sf::Vector2f(mAxes.y.getPointOfApp().x - mGraphMargin.left + viewHalfSize.x, view.getCenter().y));
    }

    mAxes.x.setPointOfApp(sf::Vector2f(view.getCenter().x - viewHalfSize.x, mAxes.x.getPointOfApp().y));
    mGraphKeys.x.setPosition(sf::Vector2f(view.getCenter().x + viewHalfSize.x - 23.f, mAxes.x.getPointOfApp().y + 5.f));

    if(mAxes.x.getPointOfApp().y - mGraphMargin.bottom < view.getCenter().y - viewHalfSize.y)
    {
        view.setCenter(sf::Vector2f(view.getCenter().x, mAxes.x.getPointOfApp().y - mGraphMargin.bottom + viewHalfSize.y));
    }
    else if(mAxes.x.getPointOfApp().y + mGraphMargin.top > view.getCenter().y + viewHalfSize.y)
    {
        view.setCenter(sf::Vector2f(view.getCenter().x, mAxes.x.getPointOfApp().y + mGraphMargin.top - viewHalfSize.y));
    }

    mAxes.y.setPointOfApp(sf::Vector2f(view.getCenter().x - viewHalfSize.x + mGraphMargin.left, view.getCenter().y + viewHalfSize.y));
    mGraphKeys.y.setPosition(sf::Vector2f(mAxes.y.getPointOfApp().x - 8.f, view.getCenter().y - viewHalfSize.y));

    this->setLastVisibleValuesIndexes(view.getCenter(), viewHalfSize);
}

void gs::Graph::updateAtViewMove(sf::View const &view)
{
    sf::Vector2f const viewCenter(view.getCenter()), viewHalfSize(view.getSize() / 2.f);

    mAxes.y.setPointOfApp(sf::Vector2f(mAxes.y.getPointOfApp().x, viewCenter.y + viewHalfSize.y));
    mGraphKeys.y.setPosition(sf::Vector2f(mAxes.y.getPointOfApp().x - 8.f, viewCenter.y - viewHalfSize.y));

    this->setLastVisibleValuesIndexes(view.getCenter(), viewHalfSize);
}

void gs::Graph::updateScale(sf::Vector2<mpf_class> const &maxValuesInDims, sf::View const &view)
{
    (this->*mScalingMethodPtr)(maxValuesInDims, view);
}

void gs::Graph::updateSymmetricScale(sf::Vector2<mpf_class> const &maxValuesInDims, sf::View const &view)
{
    sf::Vector2<mpf_class> const lens(0.85f * (view.getSize().x - mGraphMargin.left), 0.85f * (view.getSize().y - mGraphMargin.bottom));

    if(lens.x <= 0 || lens.y <= 0)
    {
        return;
    }

    sf::Vector2<mpf_class> const ticks = lens / mpf_class(mInterval);
    if(ticks.x == 0 || ticks.y == 0)
    {
        return;
    }

    pair<unsigned short, unsigned short> const unitAndExpOf10 = (maxValuesInDims.x / ticks.x > maxValuesInDims.y / ticks.y) ?
        this->findFitScale(make_pair(maxValuesInDims.x, ticks.x)) : this->findFitScale(make_pair(maxValuesInDims.y, ticks.y));

    if(   mCurrentUnit.x == unitAndExpOf10.first && mCurrentExpOf10.x == unitAndExpOf10.second
       && mCurrentUnit.y == unitAndExpOf10.first && mCurrentExpOf10.y == unitAndExpOf10.second)
    {
        return;
    }

    if(mpf_class const newScale = this->calcScale(unitAndExpOf10); newScale != mScale.x || newScale != mScale.y)
    {
        this->setNewScale(make_pair(sf::Vector2<short>(unitAndExpOf10.first, unitAndExpOf10.first),
                                    sf::Vector2<short>(unitAndExpOf10.second, unitAndExpOf10.second)),
                          sf::Vector2<mpf_class>(newScale, newScale));
    }
}

void gs::Graph::updateAsymmetricScale(sf::Vector2<mpf_class> const &maxValuesInDims, sf::View const &view)
{
    this->calcScaleX(maxValuesInDims.x, view);
    this->calcScaleY(maxValuesInDims.y, view);
}

void gs::Graph::calcScaleX(mpf_class const &maxHorizontalValue, sf::View const &view)
{
    mpf_class const horizontalLen(0.85f * (view.getSize().x - mGraphMargin.left));
    if(horizontalLen <= 0)
    {
        return;
    }

    mpf_class const ticks(horizontalLen / mpf_class(mInterval));
    if(ticks == 0)
    {
        return;
    }

    pair<unsigned short, unsigned short> const unitAndExpOf10 = this->findFitScale(make_pair(maxHorizontalValue, ticks));
    if(mCurrentUnit.x == unitAndExpOf10.first && mCurrentExpOf10.x == unitAndExpOf10.second)
    {
        return;
    }

    if(mpf_class const newScaleX = this->calcScale(unitAndExpOf10); newScaleX != mScale.x)
    {
        this->setNewScaleX(unitAndExpOf10, newScaleX);
    }
}

void gs::Graph::calcScaleY(mpf_class const &maxVerticalValue, sf::View const &view)
{
    mpf_class const verticalLen(0.85f * (view.getSize().y - mGraphMargin.bottom));
    if(verticalLen <= 0)
    {
        return;
    }

    mpf_class const ticks(verticalLen / mpf_class(mInterval));
    if(ticks == 0)
    {
        return;
    }

    pair<unsigned short, unsigned short> const unitAndExpOf10 = this->findFitScale(make_pair(maxVerticalValue, ticks));
    if(mCurrentUnit.y == unitAndExpOf10.first && mCurrentExpOf10.y == unitAndExpOf10.second)
    {
        return;
    }

    if(mpf_class const newScaleY = this->calcScale(unitAndExpOf10); newScaleY != mScale.y)
    {
        this->setNewScaleY(unitAndExpOf10, newScaleY);
    }
}

std::pair<short, short> gs::Graph::findFitScale(std::pair<mpf_class, mpf_class> const &valueAndTicks) const
{
    short i = -1;

    for(mpf_class const &s : rationalScales)
    {
        ++i;

        if(valueAndTicks.first <= s * mpf_class(valueAndTicks.second))
        {
            return make_pair(mUnits[static_cast<size_t>(i) % mUnits.size()], mExpOf10Range.first + i / static_cast<short>(mUnits.size()));
        }
    }

    for(mpz_class const &s : integerScales)
    {
        ++i;

        if(valueAndTicks.first <= mpf_class(s) * mpf_class(valueAndTicks.second))
        {
            return make_pair(mUnits[static_cast<size_t>(i) % mUnits.size()], mExpOf10Range.first + i / static_cast<short>(mUnits.size()));
        }
    }

    return make_pair(mUnits[static_cast<size_t>(i) % mUnits.size()], mExpOf10Range.first + i / static_cast<short>(mUnits.size()));
}

mpf_class gs::Graph::calcScale(std::pair<short, short> const &unitAndExpOf10)
{
    mpf_class powerOf10;
    mpf_pow_ui(powerOf10.get_mpf_t(), mpf_class(10.0).get_mpf_t(), static_cast<unsigned long>(abs(unitAndExpOf10.second)));

    if(unitAndExpOf10.second < 0)
    {
        powerOf10 = 1 / powerOf10;
    }

    return mpf_class(mInterval) / mpf_class(unitAndExpOf10.first) / powerOf10;
}

void gs::Graph::setNewScale(std::pair<sf::Vector2<short>, sf::Vector2<short>> const &unitAndExpOf10Pair, sf::Vector2<mpf_class> const &newScale)
{
    mCurrentUnit    = unitAndExpOf10Pair.first;
    mCurrentExpOf10 = unitAndExpOf10Pair.second;
    mScale          = newScale;

    this->setAllGradValues(mGradJump);
}

void gs::Graph::setNewScaleX(std::pair<short, short> const &unitAndExpOf10X, mpf_class const &newScaleX)
{
    mCurrentUnit.x    = unitAndExpOf10X.first;
    mCurrentExpOf10.x = unitAndExpOf10X.second;
    mScale.x          = newScaleX;

    this->setXGradValues(mGradJump);
}

void gs::Graph::setNewScaleY(std::pair<short, short> const &unitAndExpOf10Y, mpf_class const &newScaleY)
{
    mCurrentUnit.y    = unitAndExpOf10Y.first;
    mCurrentExpOf10.y = unitAndExpOf10Y.second;
    mScale.y          = newScaleY;

    this->setYGradValues(mGradJump);
}


void gs::Graph::setLastVisibleValuesIndexes(sf::Vector2f const &viewCenter, sf::Vector2f const &viewHalfSize)
{
    this->setLastVisibleRValuesIndex(viewCenter, viewHalfSize);
    this->setLastVisibleBValuesIndex(viewCenter, viewHalfSize);
    this->setLastVisibleTValuesIndex(viewCenter, viewHalfSize);
}

void gs::Graph::setLastVisibleRValuesIndex(sf::Vector2f const &viewCenter, sf::Vector2f const &viewHalfSize)
{
    mLastVisibleRValueIdx = static_cast<short>(viewCenter.x + viewHalfSize.x - mAxes.y.getPointOfApp().x) / static_cast<short>(mInterval) / 2 - 1;
    mLastVisibleRValueIdx-= mLastVisibleRValueIdx >= 0 ? (mRValues[mLastVisibleRValueIdx].getPosition().x + mRValues[mLastVisibleRValueIdx].getLocalBounds().width / 2.f + 1.f >= mGraphKeys.x.getPosition().x ? 1 : 0) : 0;
}

void gs::Graph::setLastVisibleBValuesIndex(sf::Vector2f const &viewCenter, sf::Vector2f const &viewHalfSize)
{
    mLastVisibleBValueIdx = static_cast<short>((viewCenter.y + viewHalfSize.y - mAxes.x.getPointOfApp().y) / static_cast<float>(mInterval)) / 2 - 1;
    mLastVisibleBValueIdx += mLastVisibleBValueIdx < static_cast<short>(mBValues.size()) - 1 ? (mBValues[mLastVisibleBValueIdx + 1].getPosition().y - mBValues[mLastVisibleBValueIdx + 1].getLocalBounds().height / 4.f < viewCenter.y + viewHalfSize.y ? 1 : 0) : 0;
}

void gs::Graph::setLastVisibleTValuesIndex(sf::Vector2f const &viewCenter, sf::Vector2f const &viewHalfSize)
{
    mLastVisibleTValueIdx = static_cast<short>(mAxes.x.getPointOfApp().y - (viewCenter.y - viewHalfSize.y)) / static_cast<short>(mInterval) / 2 - 1;
    mLastVisibleTValueIdx -= mLastVisibleTValueIdx >= 0 ? (mTValues[mLastVisibleTValueIdx].getPosition().y - mTValues[mLastVisibleTValueIdx].getLocalBounds().height / 2.f <= mGraphKeys.y.getPosition().y + mGraphKeys.y.getLocalBounds().height ? 1 : 0) : 0;
}


void gs::Graph::setAllGradValues(unsigned short const jump)
{
    this->setXGradValues(jump);
    this->setYGradValues(jump);
}

void gs::Graph::setXGradValues(unsigned short const jump)
{
    if(jump == 0)
    {
        throw invalid_argument("gs::Graph::setYGradValues: jump == 0");
    }

    short const &&unit = mCurrentUnit.x * jump;
    float const &&doubleInterval = static_cast<float>(2 * mInterval);
    sf::Vector2f positiveNumPosition(mRGrad.getPosition().x + mInterval, mRGrad.getPosition().y + 8.f);

    for(unsigned short i = 0; i < mRValues.size(); ++i)
    {
        mRValues[i].setString(gs::makeNumberStr(unit * (i + 1), mCurrentExpOf10.x, "", 5, 3));
        mRValues[i].setOrigin(sf::Vector2f(mRValues[i].getLocalBounds().left, mRValues[i].getLocalBounds().top) + sf::Vector2f(mRValues[i].getLocalBounds().width / 2.f - 1.f, 0.f));
        mRValues[i].setPosition(positiveNumPosition);

        positiveNumPosition.x += doubleInterval;
    }
}

void gs::Graph::setYGradValues(unsigned short const jump)
{
    if(jump == 0)
    {
        throw invalid_argument("gs::Graph::setYGradValues: jump == 0");
    }

    short const &&unit = mCurrentUnit.y * jump;
    float const &&doubleInterval = static_cast<float>(2 * mInterval);
    sf::Vector2f positiveNumPosition(mTGrad.getPosition().x - 8.f, mTGrad.getPosition().y - 56.f);
    sf::Vector2f negativeNumPosition(mBGrad.getPosition().x - 8.f, mBGrad.getPosition().y + 48.f);

    for(unsigned short i = 0; i < mTValues.size(); ++i)
    {
        mBValues[i].setString(makeNumberStr(unit * (i + 1), mCurrentExpOf10.y, "-", 5, 3));
        mBValues[i].setOrigin(sf::Vector2f(mBValues[i].getLocalBounds().left, mBValues[i].getLocalBounds().top) + sf::Vector2f(mBValues[i].getLocalBounds().width, mBValues[i].getLocalBounds().height / 4.f));
        mBValues[i].setPosition(negativeNumPosition);

        mTValues[i].setString(makeNumberStr(unit * (i + 1), mCurrentExpOf10.y, "", 5, 3));
        mTValues[i].setOrigin(sf::Vector2f(mTValues[i].getLocalBounds().left, mTValues[i].getLocalBounds().top) + sf::Vector2f(mTValues[i].getLocalBounds().width, mTValues[i].getLocalBounds().height / 4.f));
        mTValues[i].setPosition(positiveNumPosition);

        negativeNumPosition.y += doubleInterval;
        positiveNumPosition.y -= doubleInterval;
    }
}
