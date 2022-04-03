#ifndef __CLUA_WORKER_202203202037_H__
#define __CLUA_WORKER_202203202037_H__

#include "cworker.h"
#include "cworkerbuilder.h"
#include "luautility.h"
#include <vector>

namespace KOT
{

class CLuaWorkerBuilder : public CWorkerBuilder
{
public:
    virtual CWorker* Build();
    CLuaWorkerBuilder& AddEnvPath(const std::string& strEnvPath);
    CLuaWorkerBuilder& ClearEnvPath();
    CLuaWorkerBuilder& SetWorkerPath(const std::string& strPath);

private:
    std::vector<std::string>    m_vecEnvPath;
    std::string                 m_strWorkerPath;
};

class CLuaWorker : public CWorker
{
public:
    CLuaWorker(const std::string& stuWorkID);
    ~CLuaWorker();
    void SetWorkerPath(const std::string& strPath);
    void AddEnvPath(const std::string& strEnvPath);

protected:
    virtual void Init();
    void LoadScript();
    void OnLoad();
    virtual void OnStart();
    virtual bool OnMessage(const std::string& strMsg);
    virtual void OnTimer(int iTimerID);
    void RemeberThis();

protected:
    lua_State                   *m_pLuaState;
    std::string                 m_strPath;
};

} // namespace KOT

#endif