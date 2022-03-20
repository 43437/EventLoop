#include "cmsgqueue.h"

namespace KOT
{
    
CMsgQueue::CMsgQueue()
{

}

void CMsgQueue::QueueIn(const std::string& strMsg)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_lstMsg.push_back(strMsg);
    m_cond.notify_all();
}

bool CMsgQueue::QueueOut(std::string& strMsg)
{
    bool bRet = false;
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cond.wait(lock, [&]{return (! m_lstMsg.empty() ); });

    if (! m_lstMsg.empty() )
    {
        bRet = true;
        strMsg = m_lstMsg.front();
        m_lstMsg.pop_front();
    }
    return bRet;
}

} // namespace KOT
