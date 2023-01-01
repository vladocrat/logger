#pragma once

#include "lib.h"

TEST(GLOBAL_LOGGER, CONFIGURE)
{
    EXPECT_EQ(true, logging::FileLogger::instance()->configure("log.txt"));
}

