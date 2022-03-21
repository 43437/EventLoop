#ifndef CWORKPLACE_H
#define CWORKPLACE_H

#include <string>
#include <unordered_map>
#include <cworker.h>
#include <string>
#include <mutex>
#include "cworkerbuilder.h"

namespace KOT
{
    
class CWorkPlace
{
public:
    static CWorkPlace& GetInstance();
    void AddWorker(CWorkerBuilder& builder);
    void Message(const std::string& stuWorkerID, const std::string& strMsg);
    void Exec();

private:
    CWorkPlace();

private:
    std::unordered_map<std::string, CWorker*>               m_mapWorkers;
    mutable std::recursive_mutex                            m_RecursiveMutex;
};

} // namespace KOT

#endif // CWORKPLACE_H
