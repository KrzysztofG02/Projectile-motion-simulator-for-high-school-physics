using namespace std;


template <typename T>
gs::BaseRectangleInputFieldContainer<T>::BaseRectangleInputFieldContainer(
    sf::View const * const originViewPtr,
    std::function<void()> const &onHoverFunc,
    std::function<void()> const &onClickFunc,
    std::function<void()> const &preHoverFunc,
    std::function<void()> const &preClickFunc,
    T const &minValue,
    T const &maxValue,
    T const &currentValidValue,
    unsigned short const lenLimit,
    unsigned short const roundingDigitsCount,
    std::string const &decimalMarks):
        RectangleTextContainer(
            originViewPtr, onHoverFunc, onClickFunc, preHoverFunc, preClickFunc),
        mMinValue(minValue),
        mMaxValue(maxValue),
        mCurrentValidValue(currentValidValue),
        mLenLimit(lenLimit),
        mRoundingDigitsCount(roundingDigitsCount),
        mDecimalMarks(decimalMarks),
        mAllowedChars(string("0123456789") + string(mMinValue < 0 ? "-" : "") + decimalMarks),
        mIsCurrentValueNewBuffer(false),
        mIsCurrentValueNew(mIsCurrentValueNewBuffer),
        mIsValueApprovedBuffer(false),
        mIsValueApproved(mIsValueApprovedBuffer)
{
    this->addEvent(sf::Event::KeyPressed, bind(&BaseRectangleInputFieldContainer::pollEventKeyPressed, ref(*this), placeholders::_1));
    this->addEvent(sf::Event::MouseButtonPressed, bind(&BaseRectangleInputFieldContainer::pollEventMouseButtonPressed, ref(*this), placeholders::_1));
    this->addEvent(sf::Event::TextEntered, bind(&BaseRectangleInputFieldContainer<T>::pollEventTextEntered, ref(*this), placeholders::_1));

    this->setFont(App::arialFont);
    this->setFillColor(sf::Color::Black);

    this->setFillColor(sf::Color::White);
    this->setInnerLineColor(sf::Color::Black);
    this->setSize(sf::Vector2f(200.f, 50.f));
}


template <typename T>
void gs::BaseRectangleInputFieldContainer<T>::setInput(sf::String const &text)
{
    mContent.setString(text);

    this->validateInput();
    this->assignInputStringByCurrentValidValue();
}

template <typename T>
void gs::BaseRectangleInputFieldContainer<T>::setInput(std::string const &text)
{
    mContent.setString(text);

    this->validateInput();
    this->assignInputStringByCurrentValidValue();
}



template <typename T>
bool gs::BaseRectangleInputFieldContainer<T>::isCurrentValueNew() const
{
    return mIsCurrentValueNew;
}

template <typename T>
bool gs::BaseRectangleInputFieldContainer<T>::isValueApproved() const
{
    return mIsValueApproved;
}

template <typename T>
T gs::BaseRectangleInputFieldContainer<T>::getValue() const
{
    return mCurrentValidValue;
}

template <typename T>
unsigned short gs::BaseRectangleInputFieldContainer<T>::getRoundingDigitsCount() const
{
    return mRoundingDigitsCount;
}


template <typename T>
void gs::BaseRectangleInputFieldContainer<T>::updateIndirectly()
{
    mIsCurrentValueNew = mIsCurrentValueNewBuffer;
    mIsCurrentValueNewBuffer = false;

    mIsValueApproved = mIsValueApprovedBuffer;
    mIsValueApprovedBuffer = false;
}

template <typename T>
std::pair<bool, T> gs::BaseRectangleInputFieldContainer<T>::isNumber() const
{
    pair<bool, T> result(false, T{});
    string const inputString = this->getString().toAnsiString();

    if(   mpf_class(inputString.empty())
       || inputString.find_first_not_of(mAllowedChars) != string::npos)
    {
        return result;
    }

    if(size_t const &&lastDash = inputString.rfind('-'); lastDash != string::npos && lastDash > 0)
    {
        return result;
    }

    for(char const &m : mDecimalMarks)
    {
        if(size_t const &&no_ = count(inputString.begin(), inputString.end(), m); no_ > 1)
        {
            return result;
        }
    }

    result = make_pair(true, this->convertToNumericFromString(inputString));

    return result;
}

template <typename T>
void gs::BaseRectangleInputFieldContainer<T>::validateInput()
{
    pair<bool, T> const result = this->isNumber();

    if(result.first)
    {
        T newValidValue;

        if(result.second < mMinValue)
        {
            newValidValue = mMinValue;
        }
        else if(result.second > mMaxValue)
        {
            newValidValue = mMaxValue;
        }
        else
        {
            newValidValue = result.second;
        }

        this->round(newValidValue);

        if(mCurrentValidValue != newValidValue)
        {
            mIsCurrentValueNewBuffer = true;
            mCurrentValidValue = newValidValue;
        }
    }
}


template <typename T>
void gs::BaseRectangleInputFieldContainer<T>::pollEventKeyPressed(sf::Event const &event)
{
    if(!this->isActive())
    {
        return;
    }

    switch(event.key.code)
    {
        case sf::Keyboard::Enter:
        {
            this->validateInput();
            this->assignInputStringByCurrentValidValue();

            mIsValueApprovedBuffer = true;

            return;
        }

        case sf::Keyboard::Backspace:
        {
            if(!this->getString().isEmpty())
            {
                string buff = this->getString().toAnsiString();
                buff.pop_back();
                this->setString(buff);
            }

            return;
        }

        default: return;
    }
}

template <typename T>
void gs::BaseRectangleInputFieldContainer<T>::pollEventMouseButtonPressed(sf::Event const &event)
{
    if(event.mouseButton.button != gs::App::mainButton)
    {
        return;
    }

    if(this->contains(gs::App::mouse.getCurrentCoords(*mOriginViewPtr)))
    {
        this->setInnerLineColor(sf::Color(255, 165, 0));
    }
    else
    {
        this->setInnerLineColor(sf::Color::Black);
    }

    this->validateInput();
    this->assignInputStringByCurrentValidValue();
}

template <typename T>
void gs::BaseRectangleInputFieldContainer<T>::pollEventTextEntered(sf::Event const &event)
{
    if(!this->isActive())
    {
        return;
    }

    char const &&c = static_cast<char>(event.text.unicode);

    if(this->getString().getSize() < mLenLimit && mAllowedChars.find(c) != string::npos)
    {
        this->setString(this->getString() + c);
    }
}

template <typename T>
void gs::BaseRectangleInputFieldContainer<T>::alignContentPosition()
{
    sf::FloatRect const textBounds = mContent.getLocalBounds();

    mContent.setOrigin(textBounds.left, textBounds.top);
    mContent.setPosition(this->getPosition().x + 10.f, this->getPosition().y + (this->getSize().y - textBounds.height) / 2.f);
}
