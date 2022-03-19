#ifndef CLOGUTIL_H
#define CLOGUTIL_H

#include <string>
#include <mutex>

namespace KOT
{

#define LogUtil     CLogUtil::GetInstance()

class CLogUtil
{
public:
    static CLogUtil& GetInstance();
    void Log(const std::string& strLog);

private:
    CLogUtil();
};

} // namespace KOT

#endif // CLOGUTIL_H
