#ifndef __CLUA_WORKER_202203202037_H__
#define __CLUA_WORKER_202203202037_H__

#include "cworker.h"

namespace KOT
{

class CLuaWorker : public CWorker
{
public:
    CLuaWorker(const SWorkerID& stuWorkID);
    ~CLuaWorker();
    virtual void OnTimer(int iTimerID);

protected:
    virtual void OnMessage(const std::string& strMsg);
};

} // namespace KOT

#endif