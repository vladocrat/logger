#ifndef LOGGER_H
#define LOGGER_H

#include <unordered_map>
#include <memory>
#include <mutex>

namespace logging {

enum class Month {
    JANUARY = 1,
    FEBRUARY = 2,
    MARCH = 3,
    APRIL = 4,
    MAY = 5,
    JUNE = 6,
    JULY = 7,
    AUGUST = 8,
    SEPTEMBER = 9,
    OCTOBER = 10,
    NOVEMBER = 11,
    DECEMBER = 12
};


//not used yet?
enum class Day {
    MONDAY = 1,
    TUESDAY = 2,
    WEDNESDAY = 3,
    THURSDAY = 4,
    FRIDAY = 5,
    SATURDAY = 6,
    SUNDAY = 7
};

enum class State {
    INIT_SUCCESS,
    INIT_FAIL,
    INITIAL
};

enum class MsgType : uint8_t {
    INFO = 0,
    ERROR = 1,
    WARN = 2,
    DEFAULT = 3
};

const std::unordered_map<const MsgType, std::string, std::hash<MsgType>> logPrefixes {
    {MsgType::INFO, "[INFO]"},
    {MsgType::ERROR, "[ERROR]"},
    {MsgType::WARN, "[WARN]"},
    {MsgType::DEFAULT, ""}
};

class Logger final
{
public:
    static Logger* instance()
    {
        static Logger l;
        return &l;
    }

    //TODO complete retardness remove
    [[nodiscard]] bool configure(const std::string&);
    void log(const std::string&, const MsgType = MsgType::DEFAULT);

    const State state() const;

private:
    Logger();
    ~Logger() noexcept;
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string timestamp() const;
    std::mutex m_lock;
    void innerLog(const std::string&);

    struct File;
    std::unique_ptr<File> m_fileImpl;
    State m_state = State::INITIAL;
};

//if needed to log without marking a msg;
inline void MESSAGE(const std::string& msg) {
    Logger::instance()->log(msg, MsgType::DEFAULT);
}

inline void WARN(const std::string& msg) {
    Logger::instance()->log(msg, MsgType::WARN);
}

inline void ERROR(const std::string &msg)
{
    Logger::instance()->log(msg, MsgType::ERROR);
}

inline void INFO(const std::string &msg)
{
    Logger::instance()->log(msg, MsgType::INFO);
}


namespace utils {

//windows registery editor to track the last time file was opened

template<typename T>
auto findEnumValByInteger(const int&);

template<typename ... Args>
std::string format(const std::string& format, Args ... args);

//should really be const?
const std::string monthToStr(const Month&);
const std::string dayToStr(const Day&);

const std::unordered_map<const Month, std::string, std::hash<Month>> months {
    {Month::JANUARY, "January"},
    {Month::FEBRUARY, "February"},
    {Month::MARCH, "March"},
    {Month::APRIL, "April"},
    {Month::MAY, "May"},
    {Month::JUNE, "June"},
    {Month::JULY, "July"},
    {Month::AUGUST, "August"},
    {Month::SEPTEMBER, "September"},
    {Month::OCTOBER, "October"},
    {Month::NOVEMBER, "November"},
    {Month::DECEMBER, "December"}
};

const std::unordered_map<const Day, std::string, std::hash<Day>> days {
    {Day::MONDAY, "Monday"},
    {Day::TUESDAY, "Tuesday"},
    {Day::WEDNESDAY, "Wednesday"},
    {Day::THURSDAY, "Thursday"},
    {Day::FRIDAY, "Friday"},
    {Day::SATURDAY, "Saturday"},
    {Day::SUNDAY, "Sunday"}
};


} // UTILS

} // LOGGING
#endif // LOGGER_H
