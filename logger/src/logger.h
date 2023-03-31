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

    const constants::InitState state() const { return m_state; }
    const constants::WriteState writeState() const { return m_writeState; }

    [[nodiscard]] bool initSuccessFul() const { return m_state == constants::InitState::INIT_SUCCESS; }
    [[nodiscard]] bool writeSuccessFul() const { return m_writeState == constants::WriteState::WRITE_SUCCESS; }

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
    constants::InitState m_state = constants::InitState::INITIAL;
    constants::WriteState m_writeState = constants::WriteState::INITIAL;
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
