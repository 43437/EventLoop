#include "testall.h"
#include "cworkplace.h"
#include <iostream>
#include "cworkerbuilder.h"

namespace KOT
{

class TestWorkerBuilder : public CWorkerBuilder
{
public:
    virtual CWorker* Build()
    {
        return new CWorker(m_strWorkerID);
    }
};

void TestWorker()
{
    std::cout << "===========TestWorker==========" << std::endl;
    TestWorkerBuilder builder;
    CWorkPlace::GetInstance().AddWorker(builder.SetWorkerID("worker1"));
    CWorkPlace::GetInstance().AddWorker(builder.SetWorkerID("worker2"));
    CWorkPlace::GetInstance().AddWorker(builder.SetWorkerID("worker3"));

    CWorkPlace::GetInstance().Exec();
    std::cout << "=============================" << std::endl;
}
}