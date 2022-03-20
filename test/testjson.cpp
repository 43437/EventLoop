#include "testall.h"
#include "CJsonObject.hpp"
#include <iostream>

namespace KOT
{

void TestJson()
{
    std::cout << "===========TestJson==========" << std::endl;
    neb::CJsonObject objJson;
    objJson.Add("key", "hello");
    std::cout << objJson.ToString() << std::endl;
    std::cout << "=============================" << std::endl;
}

}