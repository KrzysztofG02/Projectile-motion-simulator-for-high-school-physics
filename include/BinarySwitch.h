#ifndef BINARYSWITCH_H
#define BINARYSWITCH_H

#include <functional>


namespace gs
{

class BinarySwitch final
{
private:
    std::function<void ()> mFunInTrueCase;
    std::function<void ()> mFunInFalseCase;
    mutable bool mState;

public:
    bool const &state;


public:
    BinarySwitch(std::function<void ()> funInTrueCase,
                 std::function<void ()> funInFalseCase,
                 bool const initialState = false);
    ~BinarySwitch() = default;
    BinarySwitch(BinarySwitch const &other);
    BinarySwitch(BinarySwitch &&other) noexcept;
    BinarySwitch & operator=(BinarySwitch const &other);
    BinarySwitch & operator=(BinarySwitch &&other) noexcept;

    void operator()() const;

    std::function<void()> getFunInTrueCase() const;
    std::function<void()> getFunInFalseCase() const;
    void setFunInTrueCase(std::function<void()> funInTrueCase);
    void setFunInFalseCase(std::function<void()> funInFalseCase);
};

}

#endif // BINARYSWITCH_H
