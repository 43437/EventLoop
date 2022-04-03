#ifndef CMSGQUEUE_H
#define CMSGQUEUE_H

#include <string>
#include <mutex>
#include <list>
#include <condition_variable>

namespace KOT
{

enum EMsgType
{
    eMsgType_From,
    eMsgType_Timer = eMsgType_From,
    eMsgType_Msg,
};

struct SMsg
{
public:
    SMsg();
    ~SMsg();
    SMsg(const KOT::SMsg&);
    SMsg& operator=(const SMsg& stuMsg);
    EMsgType            m_eMsgType;
    int                 m_iTimerID;
    std::string         m_strMsg;
};
    
class CMsgQueue
{
public:
    CMsgQueue();
    void QueueIn(const SMsg& stuMsg);
    bool QueueOut(SMsg& stuMsg);

private:
    std::mutex                      m_mutex;
    std::list<SMsg>                 m_lstMsg;
    std::condition_variable         m_cond;
};

} // namespace KOT

#endif // CMSGQUEUE_H
