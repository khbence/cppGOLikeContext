#include "gtest/gtest.h"
#include "background.hpp"

TEST(background, deadline) {
    context::Background ctx;
    EXPECT_FALSE(ctx.deadline());
}

TEST(background, done) {
    context::Background ctx;
    EXPECT_FALSE(ctx.done()());
}

TEST(background, err) {
    context::Background ctx;
    EXPECT_FALSE(ctx.err());
}

TEST(background, value) {
    context::Background ctx;
    EXPECT_FALSE(ctx.value("text").has_value());
}