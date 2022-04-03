#include "luautility.h"
#include <string>
#include <list>
#include <algorithm>
#include <iostream>
#include <functional>
#include <numeric>

namespace KOT
{
const char* KOT_LUA_LIB = "kot";
luaL_Reg utilityFunc[] = {
    {"log",     Log},
    {nullptr,   nullptr}
};

LUALIB_API int CreateLuaLib(lua_State* L)
{
    luaL_Reg arrFunc[] = {
        {nullptr, nullptr}
    };
    luaL_newlib(L, arrFunc);
    return 1;
}

void RegisterLib(lua_State* L, const char* crModName, const luaL_Reg arrFunc[])
{
    int iTop = lua_gettop(L);
    if (LUA_TTABLE != lua_getglobal(L, crModName))
    {
        //newlib create
        luaL_requiref(L, crModName, CreateLuaLib, 1);
    }

    luaL_setfuncs(L, arrFunc, 0);
    //clean the func call stack
    lua_settop(L, iTop);
}

void AddLuaEnv(lua_State* L, const char *name, const char* value)
{
    int iTop = lua_gettop(L);
    std::string v;
    lua_getglobal(L, "package");
    lua_getfield(L, -1, name);
    v.append(lua_tostring(L, -1));
    v.append(";");
    v.append(value);
    lua_pushstring(L, v.c_str());
    lua_setfield(L, -3, name);
    lua_settop(L, iTop);
}

bool CheckArray(lua_State* L, int iIndex)
{
    lua_pushnil(L);
    while (lua_next(L, iIndex) != 0)
    {
        switch (lua_type(L, -2))
        {
        case LUA_TSTRING:
            lua_pop(L, 2);
            return false;
            break;
        case LUA_TNUMBER:
            break;
        default:
            luaL_argcheck(L, false, 1, (std::string("key not support to dump: ") + lua_typename(L, -2)).c_str());
            break;
        }
        lua_pop(L, 1);
    }
    return true;
}

void LuaTable2JSON(cJSON*& pJsonNode, int iIndex, lua_State* L)
{
    lua_pushnil(L);
    if (CheckArray(L, iIndex))
    {
        pJsonNode = cJSON_CreateArray();
        while (lua_next(L, iIndex))
        {
            switch (lua_type(L, -1))
            {
            case LUA_TNIL:
                cJSON_AddItemToArray(pJsonNode, cJSON_CreateNull());
                break;
            case LUA_TBOOLEAN:
                cJSON_AddItemToArray(pJsonNode, cJSON_CreateBool(lua_toboolean(L, -1)));
                break;
            case LUA_TNUMBER:
                cJSON_AddItemToArray(pJsonNode, cJSON_CreateDouble(lua_tonumber(L, -1), -1));
                break;
            case LUA_TSTRING:
                cJSON_AddItemToArray(pJsonNode, cJSON_CreateString(lua_tostring(L, -1)));
                break;
            case LUA_TFUNCTION:
                cJSON_AddItemToArray(pJsonNode, cJSON_CreateString("function"));
                break;
            case LUA_TTABLE:
            {
                cJSON* pNewJsonNode = nullptr;
                LuaTable2JSON(pNewJsonNode, lua_gettop(L), L);
                cJSON_AddItemToArray(pJsonNode, pNewJsonNode);
            } 
                break;
            default:
                luaL_argcheck(L, false, 1, (std::string("value not support to dump: [") + lua_typename(L, -1)).c_str());
                break;
            }
            lua_pop(L, 1);
        }
    }
    else
    {
        pJsonNode = cJSON_CreateObject();
        while (lua_next(L, iIndex))
        {
            std::string strKey;
            if (lua_type(L, -2) == LUA_TSTRING)
            {
                strKey = lua_tostring(L, -2);
            }
            else
            {
                strKey = std::to_string(lua_tointeger(L, -2));
            }
            
            switch (lua_type(L, -1))
            {
            case LUA_TNIL:
                cJSON_AddItemToObject(pJsonNode, strKey.c_str(), cJSON_CreateNull());
                break;
            case LUA_TBOOLEAN:
                cJSON_AddItemToObject(pJsonNode, strKey.c_str(), cJSON_CreateBool(lua_toboolean(L, -1)));
                break;
            case LUA_TNUMBER:
                cJSON_AddItemToObject(pJsonNode, strKey.c_str(), cJSON_CreateDouble(lua_tonumber(L, -1), -1));
                break;
            case LUA_TSTRING:
                cJSON_AddItemToObject(pJsonNode, strKey.c_str(), cJSON_CreateString(lua_tostring(L, -1)));
                break;
            case LUA_TFUNCTION:
                cJSON_AddItemToObject(pJsonNode, strKey.c_str(), cJSON_CreateString("function"));
                break;
            case LUA_TTABLE:
            {
                cJSON* pNewJsonNode = nullptr;
                LuaTable2JSON(pNewJsonNode, lua_gettop(L), L);
                cJSON_AddItemToObject(pJsonNode, strKey.c_str(), pNewJsonNode);
            } 
                break;
            default:
                luaL_argcheck(L, false, 1, (std::string("value not support to dump: [") + lua_typename(L, -1)).c_str());
                break;
            }
            lua_pop(L, 1);
        }
    }
    
}

cJSON* LuaValue2JSONObj(lua_State* L)
{
    cJSON* pJsonRoot = nullptr;
    int iTop = lua_gettop(L);
    switch (lua_type(L, -1))
    {
    case LUA_TNIL:
        pJsonRoot = cJSON_CreateNull();
        break;
    case LUA_TBOOLEAN:
        pJsonRoot = cJSON_CreateBool(lua_toboolean(L, iTop));
        break;
    case LUA_TNUMBER:
        pJsonRoot = cJSON_CreateDouble(lua_tonumber(L, iTop), -1);
        break;
    case LUA_TSTRING:
        pJsonRoot = cJSON_CreateString(lua_tostring(L, iTop));
        break;
    case LUA_TFUNCTION:
        pJsonRoot = cJSON_CreateString("function");
        break;
    case LUA_TTABLE:
        LuaTable2JSON(pJsonRoot, iTop, L);
        break;
    default:
        luaL_argcheck(L, false, 1, std::string(lua_typename(L, iTop)).append(" not support to dump.").c_str());
        break;
    }
    return pJsonRoot;
}

char* LuaValue2JSON(lua_State* L, bool bIsFormat)
{
    cJSON* pJsonRoot = LuaValue2JSONObj(L);
    char* json = bIsFormat ? cJSON_Print(pJsonRoot) : cJSON_PrintUnformatted(pJsonRoot);
    cJSON_Delete(pJsonRoot);
    return json;
}

void JSON2LuaValue(cJSON* pJsonNode, lua_State* L)
{
    if (pJsonNode->string)
    {
        int iKey = atoi(pJsonNode->string);
        if (iKey)
        {
            lua_pushinteger(L, iKey);
        }
        else
        {
            lua_pushstring(L, pJsonNode->string);
        }
    }
    switch (pJsonNode->type)
    {
    case cJSON_False:
        lua_pushboolean(L, false);
        break;
    case cJSON_True:
        lua_pushboolean(L, true);
        break;
    case cJSON_Int:
    case cJSON_Double:
        lua_pushnumber(L, pJsonNode->valuedouble);
        break;
    case cJSON_String:
        lua_pushstring(L, pJsonNode->valuestring);
        break;
    case cJSON_Array:
        {
            lua_newtable(L);
            cJSON* pElement = pJsonNode->child;
            size_t iIndex = 1;
            while (pElement)
            {
                lua_pushnumber(L, iIndex++);
                JSON2LuaValue(pElement, L);
                pElement = pElement->next;
                lua_settable(L, -3);
            }
        }
        break;
    case cJSON_Object:
        {
            lua_newtable(L);
            cJSON* pElement = pJsonNode->child;
            while (pElement)
            {
                JSON2LuaValue(pElement, L);
                pElement = pElement->next;
            }
            
        }
        break;
    default:
        lua_pushnil(L);
        break;
    }
    if (pJsonNode->string)
        lua_settable(L, -3);
}

cJSON* LuaTable2JSONObj(lua_State* L)
{
    return LuaValue2JSONObj(L);
}

std::string LuaTable2JSON(lua_State* L)
{
    char* crJson = LuaValue2JSON(L, false);
    std::string json = crJson;
    free(crJson);
    return json;
}

void JSON2LuaTable(lua_State* L, const std::string& json)
{
    const char* crJson = json.c_str();
    const char* crep;
    cJSON* pJsonRoot = cJSON_Parse(crJson, &crep);
    // luaL_argcheck(L, pJsonRoot, 1, std::string("JSON parse error before: ").append(cJSON_GetE))
    JSON2LuaValue(pJsonRoot, L);
    cJSON_Delete(pJsonRoot);
}

std::string PrintTable(lua_State* L)
{
    std::string strLog("table: ");
    strLog.append(LuaTable2JSON(L));
    return strLog;
}

int Log(lua_State* L)
{
    int iTop = lua_gettop(L);
    std::list<std::string> argv;
    int iIndex = iTop;
    while (iIndex > 0)
    {
        if (LUA_TTABLE == lua_type(L, -1))
        {
            argv.push_front(PrintTable(L));
        }
        else if (LUA_TBOOLEAN == lua_type(L, -1))
        {
            if (0 == lua_toboolean(L, -1))
            {
                argv.push_front("false");
            }
            else
            {
                argv.push_front("true");
            }
        }
        else
        {
            const char* str = lua_tostring(L, -1);
            if (nullptr != str)
                argv.push_front(str);
        }
        lua_pop(L, 1);
        --iIndex;
    }

    if (!argv.empty())
    {
        std::string strLog = std::accumulate(std::next(argv.begin()), argv.end(), *(argv.begin()), [](const std::string ans, const std::string curr){
            return ans + " " + curr;
        });
        std::cout << strLog << std::endl;
    }
    else
    {
        std::cout << "Log bug no param" << std::endl;
    }
    return 0;
}

bool NoError(lua_State* L, int iRet)
{
    if (0 != iRet)
    {
        std::cout << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        return false;
    }
    else
    {
        return true;
    }
    
}

void SetPointer(lua_State* L, const std::string& strQuote, void* ptr)
{
    int iTop = lua_gettop(L);
    lua_pushlightuserdata(L, ptr);
    lua_setglobal(L, strQuote.c_str());
    lua_settop(L, iTop);
}

}