#include "cworker.h"
#include <thread>
#include "clogutil.h"
#include "cworktimermanager.h"

namespace KOT
{

CWorker::CWorkerTimerCaller::CWorkerTimerCaller(CWorker* pOwner) : CTimerCaller()
                                                                ,m_pOwner(pOwner)
{

}

void CWorker::CWorkerTimerCaller::OnTimer(int iTimerID)
{
    m_pOwner->OnTimerEvent(iTimerID);
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
    SMsg stuMsg;
    bool bLoop = true;
    OnStart();
    while (bLoop)
    {
        if ( m_objMsgQueue.QueueOut(stuMsg) )
        {
            switch (stuMsg.m_eMsgType)
            {
            case eMsgType_Timer:
                LogUtil.Log("CWorker " + m_stuWorkerID + " timer " + std::to_string(stuMsg.m_iTimerID));
                OnTimer(stuMsg.m_iTimerID);
                break;
            case eMsgType_Msg:
                LogUtil.Log("CWorker " + m_stuWorkerID + " msg " + stuMsg.m_strMsg);
                bLoop = OnMessage(stuMsg.m_strMsg);
                break;
            default:
                break;
            }
        }
    }
}

void CWorker::OnStart()
{

}

bool CWorker::OnMessage(const std::string& strMsg)
{
    return true;
}

void CWorker::OnTimer(int iTimerID)
{

}

void CWorker::Start()
{
   m_objThread = std::thread(&CWorker::Run, this);
}

void CWorker::WaitForQuit()
{
   m_objThread.join();
}

void CWorker::Message(const std::string& strMsg)
{
    SMsg stuMsg;
    stuMsg.m_eMsgType = eMsgType_Msg,
    stuMsg.m_strMsg = strMsg;
    m_objMsgQueue.QueueIn(stuMsg);
}

void CWorker::OnTimerEvent(int iTimerID)
{
    SMsg stuMsg;
    stuMsg.m_eMsgType = eMsgType_Timer,
    stuMsg.m_iTimerID = iTimerID;
    m_objMsgQueue.QueueIn(stuMsg);
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