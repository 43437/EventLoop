#include "testall.h"
#include "cworkplace.h"
#include <iostream>
#include "cworkerbuilder.h"
#include "cluaworker.h"

namespace KOT
{

void TestWorker()
{
    std::cout << "===========TestWorker==========" << std::endl;
    CLuaWorkerBuilder builder;
    builder.SetWorkerPath("../script/test/");
    // builder.AddEnvPath("../script/test/?.lua");

    // TestWorkerBuilder builder;
    CWorkPlace::GetInstance().AddWorker(builder.SetWorkerID("worker1"));
    CWorkPlace::GetInstance().AddWorker(builder.SetWorkerID("worker2"));

    CWorkPlace::GetInstance().Exec();
    std::cout << "=============================" << std::endl;
}
}