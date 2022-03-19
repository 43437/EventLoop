#ifndef __CTIMERCALLER_20220316_H__
#define __CTIMERCALLER_20220316_H__

namespace KOT
{

class CTimerCaller
{
public:
    CTimerCaller(){};
    virtual ~CTimerCaller(){};
    virtual void OnTimer(int iTimerID) = 0;
};


} // namespace KOT
#endif  //__CTIMERCALLER_20220316_H__