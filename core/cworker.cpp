#include "cworker.h"
#include <thread>
#include "clogutil.h"
#include "cworktimermanager.h"

static const std::string    WORKER_QUIT = "QUIT";

namespace KOT
{

SWorkerID::SWorkerID(long lWorkerID, const std::string strWorkerID)
    :m_lWorkerID(lWorkerID)
    ,m_strWorkerID(strWorkerID)
{
}

bool SWorkerID::operator==(const SWorkerID& other) const
{
    return (this->m_lWorkerID == other.m_lWorkerID);
}

const std::string SWorkerID::Description()
{
    return std::to_string(this->m_lWorkerID) + "_" + this->m_strWorkerID;
}

CWorker::CWorkerTimerCaller::CWorkerTimerCaller(CWorker* pOwner) : CTimerCaller()
                                                                ,m_pOwner(pOwner)
{

}

void CWorker::CWorkerTimerCaller::OnTimer(int iTimerID)
{
    m_pOwner->OnTimer(iTimerID);
}

CWorker::CWorker(const SWorkerID& stuWorkID) : m_stuWorkerID(stuWorkID)
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
        LogUtil.Log("CWorker " + m_stuWorkerID.Description() + "get Msg" + strMsg);
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
//    //test add begin
//    int iTimer = NewTimer();
//    StartTimer(iTimer, 1, 1, 1, 1);
//    //test add end
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
    // //test add begin 
    // static int i = 0;
    // ++i;
    // if (i > 5)
    // {
    //     m_objMsgQueue.QueueIn(WORKER_QUIT);
    //     return;
    // }
    // //test add end
    // m_objMsgQueue.QueueIn("timer: " + std::to_string(iTimerID));
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