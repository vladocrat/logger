#include "logger.h"

#include <fstream>
#include <iostream>
#include <mutex>
#include <ctime>

namespace logging {

static const std::string START_OF_ENTRY = "-----------------start of session-----------------\n";
static const std::string END_OF_ENTRY =  "------------------end of session------------------\n";
static const std::string NUMERICAL_FORMAT = "numerical";
static const std::string CALENDAR_FORMAT = "calendar";

struct Logger::File
{
    File(const std::string& _path)
    {
        std::cout << "logger writing to: " << _path << std::endl;
        path = _path;
        newSessionStarted = true;
    }

    ~File() noexcept
    {
        MESSAGE(END_OF_ENTRY);

        if (!file.is_open())
        {
            std::lock_guard<std::mutex> guard(mx);
            file.close();
        }
    };

    std::mutex mx;
    std::ofstream file;
    std::string path;
    bool newSessionStarted;
};

std::string Logger::timestamp() const
{
    using namespace std::chrono;

    std::string retval = "";

    auto now = system_clock::now();
    auto now_c = system_clock::to_time_t(now);
    struct tm* date = nullptr;
    localtime_s(date, &now_c);

    if (!date)
    {
        return "";
    }

    int hour  = date->tm_hour;
    int minute = date->tm_min;
    int second = date->tm_sec;
    int mon = date->tm_mon;
    int day = date->tm_mday;

    system_clock::time_point tp = system_clock::now();
    std::time_t tt = system_clock::to_time_t(tp);
    duration<double> fractional_seconds = (tp - system_clock::from_time_t(tt)) + seconds(second);

    std::string buffer("%s %02d %02d:%02d:%09.5f");
    std::string month = utils::monthToStr(static_cast<Month>(mon));
    retval = utils::format(&buffer.front(), month.c_str(), day, hour, minute, fractional_seconds.count());

    delete date;

    return retval;
}

const State Logger::state() const
{
    return m_state;
}

Logger::Logger()
{

}

Logger::~Logger() noexcept
{

}

bool Logger::configure(const std::string& path)
{
    //to make sure threads don't ruin states
    std::lock_guard<std::mutex> guard(m_lock);

    if (m_state == State::INIT_SUCCESS)
    {
        return true;
    }

    //initing pImpl
    m_fileImpl = std::make_unique<File>(path);
    m_fileImpl->file.open(path, std::ios::in | std::ios::app);

    if (!m_fileImpl->file.is_open())
    {
        m_state = State::INIT_FAIL;
        return false;
    }

    m_state = State::INIT_SUCCESS;

    return true;
}


void Logger::log(const std::string& msg, const MsgType loggingType)
{
    //TODO what if disk's size isn't empty;
    std::lock_guard<std::mutex> guard(m_fileImpl->mx);

    //if impl isnt inited then file is non-existent as well
    if (!m_fileImpl)
    {
        std::cerr << "file isn't initialized" << std::endl;
        return;
    }

    if (m_state != State::INIT_SUCCESS)
    {
        return;
    }

    //not effectinve move to init func
    if (m_fileImpl->newSessionStarted)
    {
        innerLog(START_OF_ENTRY);
        m_fileImpl->newSessionStarted = false;
    }

    std::string output;

    if (loggingType == MsgType::DEFAULT)
    {
        output.reserve(msg.length() + 1);
    }
    else
    {
        std::string ts = timestamp();
        output.reserve(msg.length() + ts.length() + 24);
        output.append(ts + " ");
        output.append(logPrefixes.find(loggingType)->second);
        output.append(": ");
    }

    output.append(msg);
    output.push_back('\n');
    innerLog(output);
}

void Logger::innerLog(const std::string& msg)
{
    //std::lock_guard<std::mutex> guard(m_fileImpl->mx);
    m_fileImpl->file << msg;
    m_fileImpl->file.flush();
}

namespace utils {

template<typename T>
auto findEnumValByInteger(const int& numVal)
{
    return static_cast<T>(numVal);
}

/*
 * c-style string must be passed as part of the second param
 * due to "C" interface;
 */
template<typename... Args>
std::string format(const std::string &format, Args... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'

    if (size_s <= 0)
    { // snprintf returns negative value if wasn't successfull
        throw std::runtime_error("Error during formatting.");
    }

    auto size = static_cast<size_t>(size_s);
    auto buf = std::make_unique<char[]>(size);
    std::snprintf(buf.get(), size, format.c_str(), args...);

    return std::string(buf.get(), buf.get() + size - 1); // remove '\0'
}


const std::string monthToStr(const Month& month)
{
    return logging::utils::months.find(month)->second;
}

const std::string dayToStr(const Day& day)
{
    return logging::utils::days.find(day)->second;
}

} //UTILS
} // LOGGING

