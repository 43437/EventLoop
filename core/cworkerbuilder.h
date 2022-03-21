#ifndef __WORKER_BUILDER_202203212134_H__
#define __WORKER_BUILDER_202203212134_H__

#include "cworker.h"
#include <string>

namespace KOT
{

class CWorkerBuilder
{
public:
    CWorkerBuilder();
    CWorkerBuilder& SetWorkerID(const std::string& strWorkerID);
    std::string GetWorkerID();
    virtual CWorker* Build() = 0;

protected:
    std::string m_strWorkerID;
};
    
} // namespace KOT

#endif