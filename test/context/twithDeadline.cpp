#include "gtest/gtest.h"
#include "contextFactory.hpp"
#include <chrono>
#include <thread>
#include <string>

using namespace std::literals::chrono_literals;

auto createWithDeadline(auto deadline) {
    return context::ContextFactory::createWithDeadlineContext(
        deadline, context::ContextFactory::createBackgroundContext());
}

TEST(deadline, allFunctionDefault) {
    auto deadline = std::chrono::system_clock::now() + 1s;
    auto ctx = createWithDeadline(deadline);
    auto actualDeadline = ctx->deadline();
    ASSERT_TRUE(actualDeadline);
    EXPECT_EQ(deadline, actualDeadline.value());
    EXPECT_FALSE(ctx->done());
    EXPECT_FALSE(ctx->err());
    EXPECT_FALSE(ctx->value("text").has_value());
    std::this_thread::sleep_until(deadline + 0.5s);
    EXPECT_TRUE(ctx->done());
    auto err = ctx->err();
    ASSERT_TRUE(err);
    EXPECT_EQ(err.value(), "deadline exceeded");
}

TEST(deadline, parentCancelled) {
    auto deadline = std::chrono::system_clock::now() + 1s;
    auto parent = context::ContextFactory::createWithCancelContext(
        context::ContextFactory::createBackgroundContext());
    auto ctx = context::ContextFactory::createWithDeadlineContext(deadline, parent);
    parent->cancel();
    EXPECT_TRUE(ctx->done());
    auto err = ctx->err();
    ASSERT_TRUE(err);
    EXPECT_EQ(err.value(), "context cancelled");
    std::this_thread::sleep_until(deadline + 0.5s);
    err = ctx->err();
    ASSERT_TRUE(err);
    EXPECT_EQ(err.value(), "context cancelled");
}

TEST(deadline, child) {
    auto deadline = std::chrono::system_clock::now() + 1s;
    auto ctx = createWithDeadline(deadline);
    auto child = context::ContextFactory::createWithCancelContext(ctx);
    std::this_thread::sleep_until(deadline + 0.5s);
    EXPECT_TRUE(child->done());
    auto err = child->err();
    ASSERT_TRUE(err);
    EXPECT_EQ(err.value(), "deadline exceeded");
}