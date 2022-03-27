#include "luawrapper.h"
#include "CJsonObject.hpp"

namespace KOT
{
extern const char* KOT_LUA_LIB;
extern luaL_Reg utilityFunc[];

void RegisterLib(lua_State* L, const char* crModName, const luaL_Reg arrFunc[]);
void AddLuaEnv(lua_State* L, const char *name, const char* value);

cJSON* LuaTable2JSONObj(lua_State* L);
std::string LuaTable2JSON(lua_State* L);
void JSON2LuaTable(lua_State* L, const std::string& json);

int Log(lua_State* L);
bool NoError(lua_State* L, int iRet);

template<class T>
T* GetPointer(lua_State* L, const std::string& strQuote)
{
    int iTop = lua_gettop(L);
    lua_getglobal(L, strQuote.c_str());
    T *p = (T*)lua_topointer(L, -1);
    lua_settop(L, iTop);
    return p;
}

void SetPointer(lua_State* L, const std::string& strQuote, void* ptr);

}