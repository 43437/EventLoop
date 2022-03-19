#include "clogutil.h"
#include <iostream>

namespace KOT
{
static std::mutex mutex;

CLogUtil::CLogUtil()
{

}

CLogUtil& CLogUtil::GetInstance()
{
    static CLogUtil _instance;
    return _instance;
}

void CLogUtil::Log(const std::string& strLog)
{
    std::lock_guard<std::mutex> lock(mutex);
    std::cout << strLog << std::endl;
}

} // namespace KOT