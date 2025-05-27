template <typename T>
constexpr gs::StatePair<T>::StatePair(T const &prevValue, T const &currValue):
    prev(prevValue), curr(currValue) {}
