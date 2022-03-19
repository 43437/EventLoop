#include "cworkplace.h"
#include "clogutil.h"

static const int ADD_WORKER_TRY = 10;

namespace KOT
{
    
CWorkPlace::CWorkPlace() : m_lSN(1)
{

}

CWorkPlace& CWorkPlace::GetInstance()
{
    static CWorkPlace _instance;
    return _instance;
}

void CWorkPlace::AddWorker(const std::string& strWorker/*= ""*/)
{
    SWorkerID stuWorkerID(m_lSN++, strWorker);
    int iTry = ADD_WORKER_TRY;
    while (iTry > 0)
    {
        if (m_mapWorkers.count(stuWorkerID) > 1)
        {
            ++stuWorkerID.m_lWorkerID;
        }
        else
        {
            LogUtil.Log("CWorkPlace::AddWorker " + stuWorkerID.Description());
            m_mapWorkers[stuWorkerID] = new CWorker(stuWorkerID);
            m_mapWorkers[stuWorkerID]->Start();
            break;
        }
    }
}

void CWorkPlace::Message(const SWorkerID& stuWorkerID, const std::string& strMsg)
{

}

void CWorkPlace::Exec()
{
    while (true)
    {
        if (m_mapWorkers.empty())
        {
            break;
        }
        const SWorkerID& stuWorkerID = m_mapWorkers.begin()->first;
        CWorker* pWorker = m_mapWorkers[stuWorkerID];
        pWorker->WaitForQuit();
        delete pWorker;
        m_mapWorkers.erase(stuWorkerID);
    }
}

} // namespace KOT