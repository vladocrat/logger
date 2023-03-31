#pragma once

#include "filelogger.h"

#include <gtest/gtest.h>

#include <string>

TEST(INPUT_READ, CHECK_STD_STRING)
{
    std::string t = "test";
    logger() << t;
    EXPECT_EQ(true, logger().writeSuccessFul());
}

TEST(INPUT_READ, CHECK_CONST_CHAR)
{
    logger() << "test";
    EXPECT_EQ(true, logger().writeSuccessFul());
}

TEST(INTPUT_READ, CHECK_INT)
{
    int i = 1;
    logger() << i;
    EXPECT_EQ(true, logger().writeSuccessFul());
}
