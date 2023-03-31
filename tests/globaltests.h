#pragma once

#include "filelogger.h"

TEST(GLOBAL_LOGGER, CONFIGURE)
{
    EXPECT_EQ(true, logging::FileLogger::instance()->configure("log.txt"));
}

