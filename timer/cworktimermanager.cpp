#include "cworktimermanager.h"
#include <iostream>
#include "clogutil.h"
#include <time.h>

static const int MAX_TIMER = 10000019;

namespace KOT
{   
    
static void timer_thread(__sigval_t val)
{
    static std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);
    STimerCallBackInfo* pTimerCallBackInfo = (STimerCallBackInfo*)val.sival_ptr;
    pTimerCallBackInfo->m_pCaller->OnTimer(pTimerCallBackInfo->m_iTimerID);
    // LogUtil.Log("timer_thread: " + std::to_string(val.sival_int));
}

CWorkTimerManager::CWorkTimerManager()
{

}

CWorkTimerManager& CWorkTimerManager::GetInstance()
{
    static CWorkTimerManager _instance;
    return _instance;
}

int CWorkTimerManager::GetFreeID()
{
    std::lock_guard<std::recursive_mutex>   lock(m_RecursiveMutex);
    int iTimerID = INVALID_ID;
    time_t t;
    time(&t);
    iTimerID = t % MAX_TIMER;
    int iTry = 10;
    while (iTry > 0)
    {
        if (Contains(iTimerID))
        {
            ++iTimerID;
        }
        else
        {
            return iTimerID;
        }
        --iTry;
    }
    return INVALID_ID;
}

int CWorkTimerManager::NewTimer(CTimerCaller* pTimerCaller)
{
    std::lock_guard<std::recursive_mutex>   lock(m_RecursiveMutex);
    int iTimerID = GetFreeID();
    if (IsValid(iTimerID))
    {
        STimerInfo stuTimerInfo;
        sigevent stuSigEvent;
        // stuSigEvent.sigev_value.sival_int = iTimerID;
        STimerCallBackInfo* pTimerCallBackInfo = new STimerCallBackInfo();
        pTimerCallBackInfo->m_iTimerID = iTimerID;
        pTimerCallBackInfo->m_pCaller = pTimerCaller;
        stuSigEvent.sigev_value.sival_ptr = pTimerCallBackInfo;
        stuSigEvent.sigev_notify = SIGEV_THREAD;
        stuSigEvent._sigev_un._sigev_thread._function = timer_thread;
        stuSigEvent._sigev_un._sigev_thread._attribute = nullptr;
        if ( 0 != timer_create(CLOCK_REALTIME, &stuSigEvent, &stuTimerInfo.m_stuTimerID) )
        {
            //create failed.
            delete pTimerCallBackInfo;
            return INVALID_ID;
        }
        else
        {
            m_mapTimer[iTimerID] = stuTimerInfo;
            m_mapTimer[iTimerID].m_pTimerCallBackInfo = pTimerCallBackInfo;
        }
    }
    else
    {
        //create failed
        return INVALID_ID;
    }
    return iTimerID;
}

bool CWorkTimerManager::SetTimer(int iTimerID, __time_t iSecStart, __time_t iSecInterval, long iNanoSecStart/* = 0*/, long iNanoSecInterval/* = 0*/)
{
    std::lock_guard<std::recursive_mutex>   lock(m_RecursiveMutex);
    if (Contains(iTimerID))
    {
        STimerInfo& stuTimerInfo = m_mapTimer[iTimerID];
        itimerspec& stuTimerSpec = stuTimerInfo.m_stuTimerSpec;
        
        stuTimerSpec.it_value.tv_sec = iSecStart;
        stuTimerSpec.it_interval.tv_sec = iSecInterval;

        stuTimerSpec.it_value.tv_nsec = iNanoSecStart;
        stuTimerSpec.it_interval.tv_nsec = iNanoSecInterval;
    }
    else
    {
        return false;
    }
    return true;
}

bool CWorkTimerManager::StartTimer(int iTimerID, __time_t iSecStart, __time_t iSecInterval, long iNanoSecStart/* = 0*/, long iNanoSecInterval/*= 0*/)
{
    std::lock_guard<std::recursive_mutex>   lock(m_RecursiveMutex);
    if (SetTimer(iTimerID, iSecStart, iSecInterval, iNanoSecStart, iNanoSecInterval))
    {
        return StartTimer(iTimerID);
    }
    return false;
}

bool CWorkTimerManager::StartTimer(int iTimerID)
{
    std::lock_guard<std::recursive_mutex>   lock(m_RecursiveMutex);
    if (Contains(iTimerID))
    {
        STimerInfo& stuTimerInfo = m_mapTimer[iTimerID];
        itimerspec& stuTimerSpec = stuTimerInfo.m_stuTimerSpec;
        if (0 != timer_settime(stuTimerInfo.m_stuTimerID, TIMER_ABSTIME, &stuTimerSpec, nullptr) )
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

void CWorkTimerManager::DestroyTimer(int iTimerID)
{
    std::lock_guard<std::recursive_mutex>   lock(m_RecursiveMutex);
    if (Contains(iTimerID))
    {
        STimerInfo& stuTimerInfo = m_mapTimer[iTimerID];
        timer_delete(stuTimerInfo.m_stuTimerID);
        delete m_mapTimer[iTimerID].m_pTimerCallBackInfo;
        m_mapTimer.erase(iTimerID);
    }
}

inline bool CWorkTimerManager::Contains(int iTimerID) const
{
    std::lock_guard<std::recursive_mutex>   lock(m_RecursiveMutex);
    if (m_mapTimer.count(iTimerID) > 0)
    {
        return true;
    }
    return false;
}

inline bool CWorkTimerManager::IsValid(int iTimerID) const
{
    if (iTimerID > 0)
    {
        return true;
    }
    return false;
}

} // namespace KOT 