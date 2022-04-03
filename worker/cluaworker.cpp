#include "cluaworker.h"
#include "cworkplace.h"
#include "clogutil.h"

namespace KOT
{

int Message(lua_State* L)
{
    bool bRet = false;
    cJSON* pJsonObj = LuaTable2JSONObj(L);
    if (nullptr != pJsonObj)
    {
        cJSON* pJsonHead = cJSON_GetObjectItem(pJsonObj, "head");
        if (nullptr != pJsonHead)
        {
            cJSON* pJsonDst = cJSON_GetObjectItem(pJsonHead, "dst");
            if (nullptr != pJsonDst && (pJsonDst->type == cJSON_String) )
            {
                bRet = true;
                std::string strDst = pJsonDst->valuestring;
                char* jsonMsg = cJSON_PrintUnformatted(pJsonObj);
                std::string strMsg = jsonMsg;
                CWorkPlace::GetInstance().Message(strDst, strMsg);
                free(jsonMsg);
            }
        }
        cJSON_Delete(pJsonObj);
    }
    if (!bRet)
    {
        std::string strMsg = LuaTable2JSON(L);
        LogUtil.Log("Lua Message failed: " + strMsg);
    }

    return 0;
}

int NewTimer(lua_State* L)
{
    int iTimterID = -1;
    CLuaWorker* pWorker = GetPointer<CLuaWorker>(L, "g_pWorker");
    if (nullptr != pWorker)
    {
        iTimterID = pWorker->NewTimer();
    }
    lua_pushinteger(L, iTimterID);
    return 1;
}

int StartTimer(lua_State* L)
{
    CLuaWorker* pWorker = GetPointer<CLuaWorker>(L, "g_pWorker");
    if (nullptr != pWorker)
    {
        int argc = lua_gettop(L);
        if (argc == 5)
        {
            int arg1 = lua_tointeger(L, 1);
            int arg2 = lua_tointeger(L, 2);
            int arg3 = lua_tointeger(L, 3);
            int arg4 = lua_tointeger(L, 4);
            int arg5 = lua_tointeger(L, 5);
            
            pWorker->StartTimer(arg1, arg2, arg3, arg4, arg5);
        }
        else
        {
            int arg1 = lua_tointeger(L, 1);
            pWorker->StartTimer(arg1);
        }
    }
    return 0;
}

int SetTimer(lua_State* L)
{
    int arg1 = lua_tointeger(L, 1);
    int arg2 = lua_tointeger(L, 2);
    int arg3 = lua_tointeger(L, 3);
    int arg4 = lua_tointeger(L, 4);
    int arg5 = lua_tointeger(L, 5);
    CLuaWorker* pWorker = GetPointer<CLuaWorker>(L, "g_pWorker");
    if (nullptr != pWorker)
    {
        pWorker->SetTimer(arg1, arg2, arg3, arg4, arg5);
    }
    return 0;
}

int StopTimer(lua_State* L)
{
    int iTimterID = lua_tointeger(L, -1);
    CLuaWorker* pWorker = GetPointer<CLuaWorker>(L, "g_pWorker");
    if (nullptr != pWorker)
    {
        iTimterID = pWorker->SetTimer(iTimterID, 0, 0, 0, 0);
    }
    return 0;
}

static luaL_Reg workerFunc[] = {
    {"message"              ,Message},
    {"timer"                ,NewTimer},
    {"start_timer"          ,StartTimer},
    {"set_timer"            ,SetTimer},
    {"stop_timer"           ,StopTimer},
    {nullptr                ,nullptr}
};

CWorker* CLuaWorkerBuilder::Build()
{
    CLuaWorker* pWorker = new CLuaWorker(m_strWorkerID);
    if (nullptr != pWorker)
    {
        for (std::vector<std::string>::const_iterator itPath = m_vecEnvPath.begin(); m_vecEnvPath.end() != itPath; ++itPath)
        {
            const std::string& strEnvPath = *itPath;
            pWorker->AddEnvPath(strEnvPath);
        }
        pWorker->SetWorkerPath(m_strWorkerPath);
    }
    return pWorker;
}

CLuaWorkerBuilder& CLuaWorkerBuilder::AddEnvPath(const std::string& strEnvPath)
{
    m_vecEnvPath.push_back(strEnvPath);
    return *this;
}

CLuaWorkerBuilder& CLuaWorkerBuilder::ClearEnvPath()
{
    m_vecEnvPath.clear();
}

CLuaWorkerBuilder& CLuaWorkerBuilder::SetWorkerPath(const std::string& strPath)
{
    m_strWorkerPath = strPath;
}

CLuaWorker::CLuaWorker(const std::string& stuWorkID) : CWorker(stuWorkID)
    ,m_pLuaState(nullptr)
    ,m_strPath("")
{
    Init();

    RemeberThis();
}

CLuaWorker::~CLuaWorker()
{

}

void CLuaWorker::RemeberThis()
{
    SetPointer(m_pLuaState, "g_pWorker", this);
}

void CLuaWorker::Init()
{
    m_pLuaState = luaL_newstate();
    luaL_openlibs(m_pLuaState);
    // KOT::AddLuaEnv(m_pLuaState, "path", "./../script/?.lua");
    KOT::RegisterLib(m_pLuaState, KOT::KOT_LUA_LIB, KOT::utilityFunc);
    KOT::RegisterLib(m_pLuaState, KOT::KOT_LUA_LIB, KOT::workerFunc);
    lua_settop(m_pLuaState, 0);
}

void CLuaWorker::LoadScript()
{
    std::string strScript = m_strPath + m_stuWorkerID + ".lua";
    NoError(m_pLuaState, luaL_dofile(m_pLuaState, strScript.c_str() ));
    lua_settop(m_pLuaState, 0);
}

void CLuaWorker::OnStart()
{
    LoadScript();

    OnLoad();
}

void CLuaWorker::OnLoad()
{
    int iTop = lua_gettop(m_pLuaState);
    lua_getglobal(m_pLuaState, "on_load");
    NoError(m_pLuaState, lua_pcall(m_pLuaState, 0, 0, 0));
    lua_settop(m_pLuaState, iTop);
}

void CLuaWorker::OnTimer(int iTimerID)
{
    int iTop = lua_gettop(m_pLuaState);
    lua_getglobal(m_pLuaState, "on_timer");
    lua_pushinteger(m_pLuaState, iTimerID);
    NoError(m_pLuaState, lua_pcall(m_pLuaState, 1, 0, 0));
    lua_settop(m_pLuaState, iTop);
}

bool CLuaWorker::OnMessage(const std::string& strMsg)
{
    bool bRet = true;
    int iTop = lua_gettop(m_pLuaState);
    lua_getglobal(m_pLuaState, "on_message");
    JSON2LuaTable(m_pLuaState, strMsg);
    if ( NoError(m_pLuaState, lua_pcall(m_pLuaState, 1, 1, 0)) )
    {
        bRet = lua_toboolean(m_pLuaState, -1);
    }
    lua_settop(m_pLuaState, iTop);
    return bRet;
}

void CLuaWorker::AddEnvPath(const std::string& strEnvPath)
{
    KOT::AddLuaEnv(m_pLuaState, "path", strEnvPath.c_str());
}

void CLuaWorker::SetWorkerPath(const std::string& strPath)
{
    m_strPath = strPath;
}

} // namespace KOT
