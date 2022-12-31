#pragma once

#include <unordered_map>
#include <memory>
#include <mutex>

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



//if needed to log without marking a msg;
inline void MESSAGE(const std::string& msg) {
    FileLogger::instance()->log(msg, constants::MsgType::DEFAULT);
}

inline void WARN(const std::string& msg) {
    FileLogger::instance()->log(msg, constants::MsgType::WARN);
}

inline void ERROR(const std::string &msg)
{
    FileLogger::instance()->log(msg, constants::MsgType::ERROR);
}

inline void INFO(const std::string &msg)
{
    FileLogger::instance()->log(msg, constants::MsgType::INFO);
}

} // LOGGING

