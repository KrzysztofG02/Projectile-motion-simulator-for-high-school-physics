template <typename TimeType>
gs::PhysicalObject<TimeType>::PhysicalObject(bool const &equilibrium):
    mEquilibrium(equilibrium) {}

template <typename TimeType>
void gs::PhysicalObject<TimeType>::update(TimeType const &timeD)
{
    if(!mEquilibrium)
    {
        this->affectObject(timeD);
    }
}

template <typename TimeType>
bool gs::PhysicalObject<TimeType>::hasEquilibrium() const
{
    return mEquilibrium;
}
