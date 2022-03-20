#include "testall.h"
#include "luautility.h"
#include "CJsonObject.hpp"
#include <iostream>

namespace KOT
{
void TestLua()
{
    std::cout << "===========TestLua==========" << std::endl;
    lua_State *pLuaState = luaL_newstate();
    luaL_openlibs(pLuaState);
    KOT::AddLuaEnv(pLuaState, "path", "./../script/?.lua");
    KOT::RegisterLib(pLuaState, KOT::KOT_LUA_LIB, KOT::utilityFunc);
    if (0 != luaL_dofile(pLuaState, "../script/test.lua") )
    {
        if (lua_isstring(pLuaState, -1))
        {
            std::cout << "luaL_dofile failed: " << lua_tostring(pLuaState, -1) << std::endl;
        }
        else
        {
            std::cout << "luaL_dofile unknow err: " << lua_tostring(pLuaState, -1) << std::endl;
        }
    }
    else
    {
        lua_settop(pLuaState, 0);

        lua_getglobal(pLuaState, "test");
        neb::CJsonObject objJson;
        neb::CJsonObject objParam;
        neb::CJsonObject objArray;
        objArray.Add(1);
        objArray.Add(-2);
        objArray.Add(3);
        objParam.Add("key", "hello");
        objJson.Add("json_param", objParam);
        objJson.Add("json_array", objArray);
        std::cout << "json param: " << objJson.ToString() << std::endl;
        KOT::JSON2LuaTable(pLuaState, objJson.ToString());
        if (0 != lua_pcall(pLuaState, 1, 0, 0))
        {
            std::cout << "lua_pcall failed: " << lua_tostring(pLuaState, -1) << std::endl;
        }
        lua_settop(pLuaState, 0);
    }
    std::cout << "=============================" << std::endl;
}
}