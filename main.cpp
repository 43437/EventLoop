#include <iostream>
#include "cworkplace.h"
#include <thread>
#include <cworktimermanager.h>
#include "CJsonObject.hpp"
extern "C"{
#include "lua.h"
#include "lauxlib.h"
}

using namespace std;
using KOT::CWorkPlace;
using KOT::CWorkTimerManager;

int main()
{
    lua_State *pLuaState = luaL_newstate();
    if ( 0 != luaL_loadstring(pLuaState, "return \"hello lua.\"") )
    {
        std::cout << "load string failed" << std::endl;
    }
    else
    {
        if (0 != lua_pcall(pLuaState, 0, 1, 0) )
        {
            std::cout << "call lua string failed." << std::endl;
        }
        else
        {
            std::string strRet = lua_tostring(pLuaState, -1);
            std::cout << "call lua string ret: " << strRet << std::endl;
        }
        
    }
    
    neb::CJsonObject objJson;
    objJson.Add("key", "hello");
    std::cout << objJson.ToString();
   CWorkPlace::GetInstance().AddWorker();
   CWorkPlace::GetInstance().AddWorker();
   CWorkPlace::GetInstance().AddWorker();

   CWorkPlace::GetInstance().Exec();

    cout << "Hello World!" << endl;
    return 0;
}
