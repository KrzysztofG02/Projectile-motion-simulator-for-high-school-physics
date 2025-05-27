#ifndef STATEPAIR_H_INCLUDED
#define STATEPAIR_H_INCLUDED

namespace gs
{

template<typename T>
struct StatePair
{
    T prev;
    T curr;

    constexpr StatePair(T const &prevValue = {}, T const &currValue = {});
};

#include "StatePair.cpp"

}

#endif // STATEPAIR_H_INCLUDED
