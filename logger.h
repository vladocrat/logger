#pragma once

#include <unordered_map>
#include <memory>
#include <mutex>
#include <fstream>

#include "Constants.h"

namespace logging {

class FileLogger final
{
public:
    static FileLogger* instance()
    {
        static FileLogger l;
        return &l;
    }

    [[nodiscard]] bool configure(const std::string&);
    void log(const std::string&, const constants::MsgType = constants::MsgType::DEFAULT);

    const constants::State state() const;

    FileLogger& logger() { return *this; }
    FileLogger& operator<<(const std::string& msg) { log(msg); return *this; }
    FileLogger& operator<<(const char* msg) { log(std::string(msg)); return *this; }
    FileLogger& operator<<(const int& msg) { log(std::to_string(msg)); return *this; }
    FileLogger& operator<<(const float& msg) { log(std::to_string(msg)); return *this; }
    FileLogger& operator<<(const double& msg) { log(std::to_string(msg)); return *this; }
    FileLogger& operator<<(const long& msg) { log(std::to_string(msg)); return *this; }

private:
    FileLogger();
    ~FileLogger() noexcept;
    FileLogger(const FileLogger&) = delete;
    FileLogger(FileLogger&&) = delete;
    FileLogger& operator=(const FileLogger&) = delete;

    std::string timestamp() const;
    std::mutex m_lock;
    void innerLog(const std::string&);

    struct File;
    std::unique_ptr<File> m_fileImpl;
    constants::State m_state = constants::State::INITIAL;
};

namespace utils {

//windows registery editor to track the last time file was opened
template<typename T>
auto findEnumValByInteger(const int&);

template<typename ... Args>
std::string format(const std::string& format, Args ... args);

//should really be const?
const std::string monthToStr(const constants::Month&);
const std::string dayToStr(const constants::Day&);

} // UTILS

} // LOGGING
