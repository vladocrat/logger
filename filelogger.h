#pragma once

#include "logger.h"

#define fileLogger logging::FileLogger::instance()

//if needed to log without marking a msg;
inline void MESSAGE(const std::string& msg)
{
    fileLogger->log(msg, logging::constants::MsgType::DEFAULT);
}

inline void WARN(const std::string& msg)
{
    fileLogger->log(msg, logging::constants::MsgType::WARN);
}

inline void ERROR(const std::string &msg)
{
    fileLogger->log(msg, logging::constants::MsgType::ERROR);
}

inline void INFO(const std::string &msg)
{
    fileLogger->log(msg, logging::constants::MsgType::INFO);
}
