#include "cluaworker.h"

namespace KOT
{

int Message(lua_State* L)
{
    return 0;
}

static luaL_Reg workerFunc[] = {
    {"message"              ,Message},
    {nullptr                ,nullptr}
};

CWorker* CLuaWorkerBuilder::Build()
{
    return new CLuaWorker(m_strWorkerID);
}

CLuaWorker::CLuaWorker(const std::string& stuWorkID) : CWorker(stuWorkID)
    ,m_pLuaState(nullptr)
{
    Init();
}

CLuaWorker::~CLuaWorker()
{

}

void CLuaWorker::Init()
{
    m_pLuaState = luaL_newstate();
    luaL_openlibs(m_pLuaState);
    KOT::AddLuaEnv(m_pLuaState, "path", "./../script/?.lua");
    KOT::RegisterLib(m_pLuaState, KOT::KOT_LUA_LIB, KOT::workerFunc);
    std::string strScript = m_stuWorkerID + ".lua";
    NoError(m_pLuaState, luaL_dofile(m_pLuaState, strScript.c_str() ));
    lua_settop(m_pLuaState, 0);
}

void CLuaWorker::OnTimer(int iTimerID)
{

}

void CLuaWorker::OnMessage(const std::string& strMsg)
{
    int iTop = lua_gettop(m_pLuaState);
    NoError(m_pLuaState, lua_getglobal(m_pLuaState, "on_message"));
    JSON2LuaTable(m_pLuaState, strMsg);
    NoError(m_pLuaState, lua_pcall(m_pLuaState, 1, 0, 0));
    lua_settop(m_pLuaState, iTop);
}

} // namespace KOT
