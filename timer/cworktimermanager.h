#ifndef CTIMER_H
#define CTIMER_H

extern "C"{
#include <time.h>
#include <signal.h>
}
#include <unordered_map>
#include <mutex>
#include "ctimercaller.h"
#include "ctimercaller.h"

namespace KOT
{
struct STimerCallBackInfo
{
    int             m_iTimerID;
    CTimerCaller    *m_pCaller;
};

struct STimerInfo
{
    /* data */
    timer_t             m_stuTimerID;
    itimerspec          m_stuTimerSpec;
    STimerCallBackInfo* m_pTimerCallBackInfo;
};
    
class CWorkTimerManager
{
public:
    static CWorkTimerManager& GetInstance();
    int NewTimer(CTimerCaller* pTimerCaller);
    //iInterval 0 for singleshot
    //ret false for no iTimerID timer
    bool SetTimer(int iTimerID, __time_t iSecStart, __time_t iSecInterval, long iNanoSecStart = 0, long iNanoSecInterval = 0);
    bool StartTimer(int iTimerID, __time_t iSecStart, __time_t iSecInterval, long iNanoSecStart = 0, long iNanoSecInterval = 0);
    //ret false for no iTimerID timer
    bool StartTimer(int iTimerID);
    void DestroyTimer(int iTimerID);
    inline bool Contains(int iTimerID) const;
    inline bool IsValid(int iTimerID) const;

private:
    CWorkTimerManager();
    int GetFreeID();

private:
    std::unordered_map<int, STimerInfo>             m_mapTimer;
    const static int                                INVALID_ID = -1;
    mutable std::recursive_mutex                    m_RecursiveMutex;

};

} // namespace KOT

#endif // CTIMER_H
