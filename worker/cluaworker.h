#ifndef __CLUA_WORKER_202203202037_H__
#define __CLUA_WORKER_202203202037_H__

#include "cworker.h"
#include "cworkerbuilder.h"

namespace KOT
{

class CLuaWorkerBuilder : public CWorkerBuilder
{
public:
    virtual CWorker* Build();
};

class CLuaWorker : public CWorker
{
public:
    CLuaWorker(const std::string& stuWorkID);
    ~CLuaWorker();
    virtual void OnTimer(int iTimerID);

protected:
    virtual void OnMessage(const std::string& strMsg);
};

} // namespace KOT

#endif