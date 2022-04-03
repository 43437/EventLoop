#include "cmsgqueue.h"

namespace KOT
{

SMsg::SMsg() : m_eMsgType(eMsgType_Msg)
{
    
}

SMsg::SMsg(const KOT::SMsg& stuMsg)
{
    this->m_eMsgType = stuMsg.m_eMsgType;
    switch (this->m_eMsgType)
    {
    case eMsgType_Timer:
        this->m_iTimerID = stuMsg.m_iTimerID;
        break;
    case eMsgType_Msg:
        this->m_strMsg = stuMsg.m_strMsg;
        break;
    default:
        break;
    }
}

SMsg::~SMsg()
{

}

SMsg& SMsg::operator=(const SMsg& stuMsg)
{
    this->m_eMsgType = stuMsg.m_eMsgType;
    switch (this->m_eMsgType)
    {
    case eMsgType_Timer:
        this->m_iTimerID = stuMsg.m_iTimerID;
        break;
    case eMsgType_Msg:
        this->m_strMsg = stuMsg.m_strMsg;
        break;
    default:
        break;
    }
    return *this;
}

CMsgQueue::CMsgQueue()
{

}

void CMsgQueue::QueueIn(const SMsg& stuMsg)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_lstMsg.push_back(stuMsg);
    m_cond.notify_all();
}

bool CMsgQueue::QueueOut(SMsg& stuMsg)
{
    bool bRet = false;
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cond.wait(lock, [&]{return (! m_lstMsg.empty() ); });

    if (! m_lstMsg.empty() )
    {
        bRet = true;
        stuMsg = m_lstMsg.front();
        m_lstMsg.pop_front();
    }
    return bRet;
}

} // namespace KOT
