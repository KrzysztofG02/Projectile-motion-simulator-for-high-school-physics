#ifndef PRESSFUNCTOR_H_INCLUDED
#define PRESSFUNCTOR_H_INCLUDED

#include <functional>


namespace gs
{

class LogicalFunctor final
{
private:
    using PredicateFunc = std::function<bool()>;
    using ExecutorFunc  = std::function<void()>;

    bool mFlag = false;
    PredicateFunc mPredicateFunc;
    ExecutorFunc mExecutorFunc;


public:
    LogicalFunctor(PredicateFunc const &predicateFunc, ExecutorFunc const &executorFunc);

    void operator()();
};

}


#endif // PRESSFUNCTOR_H_INCLUDED
