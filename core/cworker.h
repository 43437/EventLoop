#ifndef CWORKER_H
#define CWORKER_H

#include "cmsgqueue.h"
#include <thread>
#include "ctimercaller.h"
#include <set>

namespace KOT
{

class CWorker
{
public:
    CWorker(const std::string& stuWorkID);
    virtual ~CWorker();
    virtual void Start();
    virtual void Message(const std::string& strMsg);
    void WaitForQuit();
    virtual void OnTimerEvent(int iTimerID);
    int NewTimer();
    bool SetTimer(int iTimerID, __time_t iSecStart, __time_t iSecInterval, long iNanoSecStart = 0, long iNanoSecInterval = 0);
    bool StartTimer(int iTimerID, __time_t iSecStart, __time_t iSecInterval, long iNanoSecStart = 0, long iNanoSecInterval = 0);
    bool StartTimer(int iTimerID);

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
    virtual bool OnMessage(const std::string& strMsg);
    inline void DestroyTimer(int iTimerID);
    inline void DestroyTimers();
    inline bool IsValidTimerID(int iTimerID);
    virtual void OnTimer(int iTimerID);

protected:
    CMsgQueue               m_objMsgQueue;
    std::thread             m_objThread;
    std::string             m_stuWorkerID;
    CWorkerTimerCaller      *m_pTimerCaller;
    std::set<int>           m_setTimerID;
};

} // namespace KOT

#endif // CWORKER_H