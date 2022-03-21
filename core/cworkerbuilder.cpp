#include "cworkerbuilder.h"

namespace KOT
{

CWorkerBuilder::CWorkerBuilder() : m_strWorkerID("")
{
    
}

CWorkerBuilder& CWorkerBuilder::SetWorkerID(const std::string& strWorkerID)
{
    m_strWorkerID = strWorkerID;
    return *this;
}

std::string CWorkerBuilder::GetWorkerID()
{
    return m_strWorkerID;
}

} // namespace KOT
