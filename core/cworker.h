#ifndef CWORKER_H
#define CWORKER_H

#include "cmsgqueue.h"
#include <thread>
#include "ctimercaller.h"
#include <set>

namespace KOT
{
    
struct SWorkerID
{
    long            m_lWorkerID;
    std::string     m_strWorkerID;
    SWorkerID(long lWorkerID, const std::string strWorkerID);

    bool operator==(const SWorkerID& other) const;
    const std::string Description();
};

struct SWorkerIDHash
{
    size_t operator()(const SWorkerID& stuWorkerID) const
    {
        return stuWorkerID.m_lWorkerID;
    }
};

class CWorker
{
public:
    CWorker(const SWorkerID& stuWorkID);
    virtual ~CWorker();
    virtual void Start();
    virtual void Message(const std::string& string);
    void Quit();
    void WaitForQuit();
    virtual void OnTimer(int iTimerID);

protected:
    class CWorkerTimerCaller : public CTimerCaller
    {
    public:
        CWorkerTimerCaller(CWorker* pOwner);
        virtual void OnTimer(int iTimerID);
    private:
        CWorker*        m_pOwner;
    };

protected:
    virtual void Run();
    virtual void OnMessage(const std::string& strMsg);
    int NewTimer();
    bool SetTimer(int iTimerID, __time_t iSecStart, __time_t iSecInterval, long iNanoSecStart = 0, long iNanoSecInterval = 0);
    bool StartTimer(int iTimerID, __time_t iSecStart, __time_t iSecInterval, long iNanoSecStart = 0, long iNanoSecInterval = 0);
    bool StartTimer(int iTimerID);
    inline void DestroyTimer(int iTimerID);
    inline void DestroyTimers();
    inline bool IsValidTimerID(int iTimerID);

protected:
    CMsgQueue               m_objMsgQueue;
    std::thread             m_objThread;
    SWorkerID               m_stuWorkerID;
    CWorkerTimerCaller      *m_pTimerCaller;
    std::set<int>           m_setTimerID;
};

} // namespace KOT

#endif // CWORKER_H