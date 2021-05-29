#include "gtest/gtest.h"
#include "contextFactory.hpp"
#include "background.hpp"
#include "todo.hpp"
#include "withCancel.hpp"
#include "withDeadline.hpp"
#include "withValue.hpp"
#include <chrono>

using namespace std::literals::chrono_literals;

TEST(factory, background) {
    auto ctx = context::ContextFactory::createBackgroundContext();
    EXPECT_TRUE(dynamic_cast<context::Background*>(ctx.get()));
}

TEST(factory, todo) {
    auto ctx = context::ContextFactory::createTODOContext();
    EXPECT_TRUE(dynamic_cast<context::TODO*>(ctx.get()));
}

TEST(factory, withCancel) {
    auto ctx = context::ContextFactory::createWithCancelContext(
        context::ContextFactory::createBackgroundContext());
    EXPECT_TRUE(dynamic_cast<context::WithCancel*>(ctx.get()));
}

TEST(factory, withValue) {
    auto key = 1;
    auto value = 2;
    auto ctx = context::ContextFactory::createWithValueContext(
        key, value, context::ContextFactory::createBackgroundContext());
    auto retValue = ctx->value(5);
    EXPECT_FALSE(retValue.has_value());
    retValue = ctx->value(1);
    EXPECT_TRUE(retValue.has_value());
    decltype(value) val;
    ASSERT_NO_THROW(val = std::any_cast<decltype(value)>(retValue););
    EXPECT_EQ(val, value);
}

TEST(factory, withDeadline) {
    auto deadline = std::chrono::system_clock::now() + 3s;
    auto ctx = context::ContextFactory::createWithDeadlineContext(
        deadline, context::ContextFactory::createBackgroundContext());
    EXPECT_TRUE(dynamic_cast<context::WithDeadline*>(ctx.get()));
    ASSERT_TRUE(ctx->deadline().has_value());
    auto actual = ctx->deadline().value();
    EXPECT_EQ(deadline, actual);
}

TEST(factory, withTimeout) {
    auto deadline = std::chrono::system_clock::now() + 3s;
    auto ctx = context::ContextFactory::createWithTimeoutContext(
        3s, context::ContextFactory::createBackgroundContext());
    EXPECT_TRUE(dynamic_cast<context::WithDeadline*>(ctx.get()));
    ASSERT_TRUE(ctx->deadline().has_value());
    auto actual = ctx->deadline().value();
    EXPECT_TRUE((deadline - 0.5s < actual) && (deadline + 0.5s > actual));
}
