#include "logger.h"

#include <QFile>
#include <QTextStream>
#include <mutex>
#include <QString>
#include <ctime>
#include <QDateTime>
#include <QDebug>

namespace logging {

static const std::string START_OF_ENTRY = "-----------------start of session-----------------\n";
static const std::string END_OF_ENTRY =  "------------------end of session------------------\n";
static const std::string NUMERICAL_FORMAT = "numerical";
static const std::string CALENDAR_FORMAT = "calendar";

struct Logger::File {
    File(const QString& path) : m_file(path) {
        qDebug() << "logger is going to write to: " << path;
        newSessionStarted = true;
    }

    ~File() noexcept {
        MESSAGE(END_OF_ENTRY);

        std::lock_guard<std::mutex> guard(mx);
        m_file.close();
    };

    std::mutex mx;
    QFile m_file;
    bool newSessionStarted;
};

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

std::string Logger::timestamp() const
{
    using namespace std::chrono;

    std::string retval = "";

    if (DATE_FORMAT == CALENDAR_FORMAT) {
        QDate date(QDate::currentDate());
        QTime t(QTime::currentTime());
        int hour  = t.hour();
        int minute = t.minute();
        int second = t.second();
        int mon = date.month();
        int day = date.day();

        //unable to get rid of "C" api...
        system_clock::time_point tp = system_clock::now();
        std::time_t tt = system_clock::to_time_t(tp);
        duration<double> fractional_seconds = (tp - system_clock::from_time_t(tt)) + seconds(second);

        std::string buffer("%s %02d %02d:%02d:%09.5f");
        std::string month = utils::monthToStr(static_cast<Month>(mon));
        retval = utils::format(&buffer.front(), month.c_str(), day, hour, minute, fractional_seconds.count());
    } else { //since there are only two formats avliable and if non stated it can be used as default;
        retval = QDateTime::currentDateTime().toString("MM/dd hh:mm:ss.zzzzz").toStdString();
    }

    return retval;
}

State Logger::state() const
{
    return m_state;
}

Logger::Logger()
{
    m_state = configure() ? State::INIT_SUCCESS : State::INIT_FAIL;
}

Logger::~Logger() noexcept
{

}

bool Logger::configure()
{
    std::lock_guard<std::mutex> guard(m_lock);

    //to make sure threads don't ruin states
    if (m_state == State::INIT_SUCCESS) {
        return true;
    }

    //initing pImpl
    m_fileImpl = std::make_unique<File>(QString::fromStdString(LOG_FILE_PATH));

    //if failed to open a file
    if (!m_fileImpl->m_file.exists()) {
        return false;
    }

    if (!m_fileImpl->m_file.open(QIODevice::WriteOnly |
                                 QIODevice::Text |
                                 QIODevice::Append)) {
        return false;
    }

    return true;
}


void Logger::log(const std::string& msg, const MsgType loggingType)
{
    //TODO what if disk's size isn't empty;
    m_fileImpl->mx.lock();
    //if impl isnt inited then file is non-existent as well
    if (!m_fileImpl) {
        qDebug() << "file isn't initialized";
        return;
    }

    if (m_state != State::INIT_SUCCESS) {
        return;
    }

    if (m_fileImpl->newSessionStarted) {
        QTextStream out(&m_fileImpl->m_file);
        out << QString::fromStdString(START_OF_ENTRY);
        m_fileImpl->m_file.flush();
        m_fileImpl->newSessionStarted = false;
    }
    m_fileImpl->mx.unlock();

    std::string output;
    if (loggingType == MsgType::DEFAULT) {
        output.reserve(msg.length() + 1);
    } else {
        std::string ts = timestamp();
        output.reserve(msg.length() + ts.length() + 24);
        output.append(ts + " ");
        output.append(logPrefixes.find(loggingType)->second);
        output.append(": ");
    }

    output.append(msg);
    output.push_back('\n');
    log(output);
}

void Logger::log(const std::string& msg)
{
    std::lock_guard<std::mutex> guard(m_fileImpl->mx);
    QTextStream out(&m_fileImpl->m_file);
    out << QString::fromStdString(msg);
    m_fileImpl->m_file.flush();
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
    if (size_s <= 0) { // snprintf returns negative value if wasn't successfull
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

