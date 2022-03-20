#include "testall.h"
#include "cworkplace.h"
#include <iostream>

namespace KOT
{
void TestWorker()
{
    std::cout << "===========TestWorker==========" << std::endl;
    CWorkPlace::GetInstance().AddWorker();
    CWorkPlace::GetInstance().AddWorker();
    CWorkPlace::GetInstance().AddWorker();

    CWorkPlace::GetInstance().Exec();
    std::cout << "=============================" << std::endl;
}
}