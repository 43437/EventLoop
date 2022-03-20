#include "luawrapper.h"
#include "CJsonObject.hpp"

namespace KOT
{
extern const char* KOT_LUA_LIB;
extern luaL_Reg utilityFunc[];

void RegisterLib(lua_State* L, const char* crModName, const luaL_Reg arrFunc[]);
void AddLuaEnv(lua_State* L, const char *name, const char* value);

std::string LuaTable2JSON(lua_State* L);
void JSON2LuaTable(lua_State* L, const std::string& json);

int Log(lua_State* L);
}