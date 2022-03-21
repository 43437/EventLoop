#include "cluaworker.h"

namespace KOT
{

CWorker* CLuaWorkerBuilder::Build()
{
    return new CLuaWorker(m_strWorkerID);
}

CLuaWorker::CLuaWorker(const std::string& stuWorkID) : CWorker(stuWorkID)
{

}

CLuaWorker::~CLuaWorker()
{

}

void CLuaWorker::OnTimer(int iTimerID)
{

}

void CLuaWorker::OnMessage(const std::string& strMsg)
{

}

} // namespace KOT
