#include "cworker.h"
#include <thread>
#include "clogutil.h"
#include "cworktimermanager.h"

static const std::string    WORKER_QUIT = "QUIT";

namespace KOT
{

CWorker::CWorkerTimerCaller::CWorkerTimerCaller(CWorker* pOwner) : CTimerCaller()
                                                                ,m_pOwner(pOwner)
{

}

void CWorker::CWorkerTimerCaller::OnTimer(int iTimerID)
{
    m_pOwner->OnTimer(iTimerID);
}

CWorker::CWorker(const std::string& stuWorkID) : m_stuWorkerID(stuWorkID)
{
    m_pTimerCaller = new CWorkerTimerCaller(this);
}

CWorker::~CWorker()
{
    DestroyTimers();
}

void CWorker::Run()
{
    std::string strMsg("");
    while (true)
    {
        m_objMsgQueue.QueueOut(strMsg);
        LogUtil.Log("CWorker " + m_stuWorkerID + "get Msg" + strMsg);
        if (WORKER_QUIT == strMsg)
        {
            break;
        }
        else
        {
            OnMessage(strMsg);
        }
        
    }
}

void CWorker::OnMessage(const std::string& strMsg)
{

}

void CWorker::Start()
{
   m_objThread = std::thread(&CWorker::Run, this);
}

void CWorker::Quit()
{
    m_objMsgQueue.QueueIn(WORKER_QUIT);
}

void CWorker::WaitForQuit()
{
   m_objThread.join();
}

void CWorker::Message(const std::string& string)
{
    m_objMsgQueue.QueueIn(string);
}

void CWorker::OnTimer(int iTimerID)
{

}

int CWorker::NewTimer()
{
    int iTimer = CWorkTimerManager::GetInstance().NewTimer(m_pTimerCaller);
    if(IsValidTimerID(iTimer))
    {
        m_setTimerID.insert(iTimer);
    }
    return iTimer;
}

bool CWorker::SetTimer(int iTimerID, __time_t iSecStart, __time_t iSecInterval, long iNanoSecStart/* = 0*/, long iNanoSecInterval/* = 0*/)
{
    return CWorkTimerManager::GetInstance().SetTimer(iTimerID, iSecStart, iSecInterval, iNanoSecStart, iNanoSecInterval);
}

bool CWorker::StartTimer(int iTimerID, __time_t iSecStart, __time_t iSecInterval, long iNanoSecStart/* = 0*/, long iNanoSecInterval/* = 0*/)
{
    return CWorkTimerManager::GetInstance().StartTimer(iTimerID, iSecStart, iSecInterval, iNanoSecStart, iNanoSecInterval);
}

bool CWorker::StartTimer(int iTimerID)
{
    return CWorkTimerManager::GetInstance().StartTimer(iTimerID);
}

inline void CWorker::DestroyTimer(int iTimerID)
{
    CWorkTimerManager::GetInstance().DestroyTimer(iTimerID);
}

inline void CWorker::DestroyTimers()
{
    for (auto it = m_setTimerID.begin(); m_setTimerID.end() != it; ++it)
    {
        DestroyTimer(*it);
    }
    m_setTimerID.clear();
}

inline bool CWorker::IsValidTimerID(int iTimerID)
{
    return (CWorkTimerManager::GetInstance().IsValid(iTimerID));
}

} // namespace KOT