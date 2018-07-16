#ifndef __LOG_H__
#define __LOG_H__

#include <sstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <chrono>
 using std::chrono::time_point;
 using std::chrono::system_clock;
 using std::chrono::duration;
 using std::chrono::duration_cast;
 using std::ratio_multiply;
 using std::ratio;
#include "debug_utils.h"
inline std::string NowTime()
{
    time_point<system_clock> time = system_clock::now();
    auto allDuration = time.time_since_epoch();
    typedef duration<int, ratio_multiply<std::chrono::hours::period,
            ratio<24>>::type> Days;
    Days days = duration_cast<Days>(allDuration);
    allDuration -= days;
    auto hours = duration_cast<std::chrono::hours>(allDuration);
    allDuration -= hours;
    auto minutes = duration_cast<std::chrono::minutes>(allDuration);
    allDuration -= minutes;
    auto seconds = duration_cast<std::chrono::seconds>(allDuration);
    allDuration -= seconds;
    auto milliseconds = duration_cast<std::chrono::milliseconds>(allDuration);
    allDuration -= milliseconds;

    std::stringstream tmp;
    tmp << hours.count() << ":" << minutes.count() << ":"
        << seconds.count() << "."
        << std::setfill('0') << std::setw(3) << milliseconds.count();

    std::string out;
    tmp >> out;
    return out;
}

enum TLogLevel {logERROR, logWARNING, logINFO, logDEBUG, logDEBUG1, logDEBUG2, logDEBUG3, logDEBUG4};

template <typename T>
class Log
{
public:
    Log();
    virtual ~Log();
    std::ostringstream& Get(TLogLevel level = logINFO);
public:
    static TLogLevel& ReportingLevel();
    static std::string ToString(TLogLevel level);
    static TLogLevel FromString(const std::string& level);
protected:
    std::ostringstream os;
private:
    Log(const Log&);
    Log& operator =(const Log&);
};

template <typename T>
Log<T>::Log()
{
}

template <typename T>
std::ostringstream& Log<T>::Get(TLogLevel level)
{
    os << "[" << NowTime() << " UTC";
    os << std::string(level > logDEBUG ? level - logDEBUG : 0, '\t') << "] "
       << "[" << ToString(level) << "] ";

    return os;
}

template <typename T>
Log<T>::~Log()
{
    os << std::endl;
    T::Output(os.str());
}

template <typename T>
TLogLevel& Log<T>::ReportingLevel()
{
    static TLogLevel reportingLevel = logDEBUG4;
    return reportingLevel;
}

template <typename T>
std::string Log<T>::ToString(TLogLevel level)
{
	static const char* const buffer[] = {"ERROR", "WARNING", "INFO", "DEBUG", "DEBUG1", "DEBUG2", "DEBUG3", "DEBUG4"};
    return buffer[level];
}

template <typename T>
TLogLevel Log<T>::FromString(const std::string& level)
{
    if (level == "DEBUG4")
        return logDEBUG4;
    if (level == "DEBUG3")
        return logDEBUG3;
    if (level == "DEBUG2")
        return logDEBUG2;
    if (level == "DEBUG1")
        return logDEBUG1;
    if (level == "DEBUG")
        return logDEBUG;
    if (level == "INFO")
        return logINFO;
    if (level == "WARNING")
        return logWARNING;
    if (level == "ERROR")
        return logERROR;
    Log<T>().Get(logWARNING) << "Unknown logging level '" << level << "'. Using INFO level as default.";
    return logINFO;
}

class Output2FILE
{
public:
    static FILE*& Stream();
    static void Output(const std::string& msg);
};

inline FILE*& Output2FILE::Stream()
{
    static FILE* pStream = stderr;
    return pStream;
}

inline void Output2FILE::Output(const std::string& msg)
{
    FILE* pStream = Stream();
    if (!pStream)
        return;
    fprintf(pStream, "%s", msg.c_str());
    fflush(pStream);
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#   if defined (BUILDING_FILELOG_DLL)
#       define FILELOG_DECLSPEC   __declspec (dllexport)
#   elif defined (USING_FILELOG_DLL)
#       define FILELOG_DECLSPEC   __declspec (dllimport)
#   else
#       define FILELOG_DECLSPEC
#   endif // BUILDING_DBSIMPLE_DLL
#else
#   define FILELOG_DECLSPEC
#endif // _WIN32

class FILELOG_DECLSPEC FILELog : public Log<Output2FILE> {};
//typedef Log<Output2FILE> FILELog;

#ifndef FILELOG_MAX_LEVEL
#define FILELOG_MAX_LEVEL logDEBUG4
#endif

#define FILE_LOG(level) \
    if (level == logDEBUG4) {\
        LOG();\
    }\
    if (level > FILELOG_MAX_LEVEL) ;\
    else if (level > FILELog::ReportingLevel() || !Output2FILE::Stream()) ; \
    else FILELog().Get(level)

#endif //__LOG_H__
