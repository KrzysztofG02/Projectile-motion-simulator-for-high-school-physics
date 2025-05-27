#include "LogicalFunctor.h"


gs::LogicalFunctor::LogicalFunctor(PredicateFunc const &predicateFunc, ExecutorFunc const &executorFunc):
    mPredicateFunc(predicateFunc), mExecutorFunc(executorFunc) {}

void gs::LogicalFunctor::operator()()
{
    if(mPredicateFunc())
    {
        if(!mFlag)
        {
            mExecutorFunc();

            mFlag = true;
        }
    }
    else if(mFlag)
    {
        mFlag = false;
    }
}
