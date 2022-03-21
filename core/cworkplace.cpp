#include "cworkplace.h"
#include "clogutil.h"

static const int ADD_WORKER_TRY = 10;

namespace KOT
{
    
CWorkPlace::CWorkPlace()
{

}

CWorkPlace& CWorkPlace::GetInstance()
{
    static CWorkPlace _instance;
    return _instance;
}

void CWorkPlace::AddWorker(CWorkerBuilder& builder)
{
    std::unique_lock<std::recursive_mutex>  lock(m_RecursiveMutex);
    if (m_mapWorkers.count(builder.GetWorkerID()) > 0)
    {
        LogUtil.Log("CWorkPlace::AddWorker " + builder.GetWorkerID() + "exsit.");
    }
    else
    {
        LogUtil.Log("CWorkPlace::AddWorker " + builder.GetWorkerID());
        m_mapWorkers[builder.GetWorkerID()] = builder.Build();
        m_mapWorkers[builder.GetWorkerID()]->Start();
    }
}

void CWorkPlace::Message(const std::string& stuWorkerID, const std::string& strMsg)
{
    {
        std::unique_lock<std::recursive_mutex>  lock(m_RecursiveMutex);
        if (m_mapWorkers.count(stuWorkerID) > 0)
        {
            CWorker* pWorker = m_mapWorkers[stuWorkerID];
            pWorker->Message(strMsg);
        }
        else
        {
            LogUtil.Log("CWorkPlace::Message no worker:" + stuWorkerID);
        }
    }
}

void CWorkPlace::Exec()
{
    while (true)
    {
        if (m_mapWorkers.empty())
        {
            break;
        }
        const std::string& stuWorkerID = m_mapWorkers.begin()->first;
        CWorker* pWorker = m_mapWorkers[stuWorkerID];
        pWorker->WaitForQuit();
        {
            std::unique_lock<std::recursive_mutex>  lock(m_RecursiveMutex);
            delete pWorker;
            m_mapWorkers.erase(stuWorkerID);
        }
    }
}

} // namespace KOT