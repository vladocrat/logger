#pragma once

#include <string>
#include <unordered_map>

namespace logging {

namespace constants {

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

} // CONSTANTS

} // LOGGING
