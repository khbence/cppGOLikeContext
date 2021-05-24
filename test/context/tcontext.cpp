#include "context.hpp"
#include "gtest/gtest.h"
#include <thread>

TEST(Token, defaultConstructor) {
    context::Token t;
    EXPECT_FALSE(t());
}

TEST(Token, tokenConstructor) {
    std::stop_source src;
    context::Token t{src.get_token()};
    EXPECT_FALSE(t());
    src.request_stop();
    EXPECT_TRUE(t());
}

TEST(Token, copyToken) {
    std::stop_source src;
    context::Token t{src.get_token()};
    auto copy = t;
    EXPECT_FALSE(copy());
    EXPECT_FALSE(t());
    src.request_stop();
    EXPECT_TRUE(copy());
    EXPECT_TRUE(t());
}