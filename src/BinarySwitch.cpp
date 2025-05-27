#include "BinarySwitch.h"

using namespace std;


gs::BinarySwitch::BinarySwitch(std::function<void ()> funInTrueCase,
                               std::function<void ()> funInFalseCase,
                               bool const initialState):
    mFunInTrueCase(funInTrueCase),
    mFunInFalseCase(funInFalseCase),
    mState(initialState),
    state(mState) {}

gs::BinarySwitch::BinarySwitch(BinarySwitch const &other):
    mFunInTrueCase(other.mFunInTrueCase),
    mFunInFalseCase(other.mFunInFalseCase),
    mState(other.mState),
    state(mState) {}

gs::BinarySwitch::BinarySwitch(BinarySwitch &&other) noexcept:
    mFunInTrueCase(move(other.mFunInTrueCase)),
    mFunInFalseCase(move(other.mFunInFalseCase)),
    mState(move(other.mState)),
    state(mState) {}

gs::BinarySwitch & gs::BinarySwitch::operator=(BinarySwitch const &other)
{
    mFunInTrueCase = other.mFunInTrueCase;
    mFunInFalseCase = other.mFunInFalseCase;
    mState = other.mState;

    return *this;
}

gs::BinarySwitch & gs::BinarySwitch::operator=(BinarySwitch &&other) noexcept
{
    mFunInTrueCase = move(other.mFunInTrueCase);
    mFunInFalseCase = move(other.mFunInFalseCase);
    mState = move(other.mState);

    return *this;
}


void gs::BinarySwitch::operator()() const
{
    if(mState)
    {
        mFunInTrueCase();
    }
    else
    {
        mFunInFalseCase();
    }

    mState = !mState;
}


std::function<void()> gs::BinarySwitch::getFunInTrueCase() const
{
    return mFunInTrueCase;
}

std::function<void()> gs::BinarySwitch::getFunInFalseCase() const
{
    return mFunInFalseCase;
}

void gs::BinarySwitch::setFunInTrueCase(std::function<void()> funInTrueCase)
{
    mFunInTrueCase = funInTrueCase;
}

void gs::BinarySwitch::setFunInFalseCase(std::function<void()> funInFalseCase)
{
    mFunInFalseCase = funInFalseCase;
}
