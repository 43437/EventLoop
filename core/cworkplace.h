#ifndef CWORKPLACE_H
#define CWORKPLACE_H

#include <string>
#include <unordered_map>
#include <cworker.h>
#include <string>

namespace KOT
{
    
class CWorkPlace
{
public:
    static CWorkPlace& GetInstance();
    void AddWorker(const std::string& strWorker = "");
    void Message(const SWorkerID& stuWorkerID, const std::string& strMsg);
    void Exec();

private:
    CWorkPlace();

private:
    long                                    m_lSN;
    std::unordered_map<SWorkerID, CWorker*, SWorkerIDHash> m_mapWorkers;
};

} // namespace KOT

#endif // CWORKPLACE_H
