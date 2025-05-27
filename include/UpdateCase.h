#ifndef UPDATECASE_H_INCLUDED
#define UPDATECASE_H_INCLUDED


namespace gs
{

enum class UpdateCase: char
{
    none,
    windowResize,
    paramsChange,
    mouseHoldMove,
    resize,
    motion
};

}

#endif // UPDATECASE_H_INCLUDED
