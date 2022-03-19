#ifndef CMSGQUEUE_H
#define CMSGQUEUE_H

#include <string>
#include <mutex>
#include <list>
#include <condition_variable>

namespace KOT
{
    
class CMsgQueue
{
public:
    CMsgQueue();
    void QueueIn(const std::string& strMsg);
    bool QueueOut(std::string& strMsg);

private:
    std::mutex                      m_mutex;
    std::list<std::string>          m_lstMsg;
    std::condition_variable         m_cond;
};

} // namespace KOT

#endif // CMSGQUEUE_H
