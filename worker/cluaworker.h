#ifndef __CLUA_WORKER_202203202037_H__
#define __CLUA_WORKER_202203202037_H__

#include "cworker.h"
#include "cworkerbuilder.h"
#include "luautility.h"

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

protected:
    virtual void Init();
    void OnLoad();
    virtual bool OnMessage(const std::string& strMsg);
    virtual void OnTimer(int iTimerID);
    void RemeberThis();

protected:
    lua_State                   *m_pLuaState;
};

} // namespace KOT

#endif