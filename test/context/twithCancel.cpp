#include "gtest/gtest.h"
#include "contextFactory.hpp"
#include <array>
#include <string>

auto createWithCancel() {
    return context::ContextFactory::createWithCancelContext(
        context::ContextFactory::createBackgroundContext());
}

TEST(withCancel, allFunctionDefault) {
    auto ctx = createWithCancel();
    EXPECT_FALSE(ctx->deadline());
    EXPECT_FALSE(ctx->done());
    EXPECT_FALSE(ctx->err());
    EXPECT_FALSE(ctx->value("text").has_value());
}

TEST(withCancel, singleCancel) {
    auto ctx = createWithCancel();
    EXPECT_FALSE(ctx->done());
    ctx->cancel();
    EXPECT_TRUE(ctx->done());
}

TEST(withCancel, cancelPropagation) {
    constexpr unsigned N = 5;
    auto ctx = createWithCancel();
    std::array<decltype(ctx), N> contexts{ ctx };
    for (unsigned i = 1; i < N; ++i) {
        contexts[i] = context::ContextFactory::createWithCancelContext(contexts[i - 1]);
    }
    constexpr unsigned C = 2;
    contexts[C]->cancel();
    for (unsigned i = 0; i < C; ++i) {
        EXPECT_FALSE(contexts[i]->done());
        EXPECT_FALSE(contexts[i]->err());
    }
    for (unsigned i = C; i < N; ++i) {
        EXPECT_TRUE(contexts[i]->done());
        auto err = contexts[i]->err();
        ASSERT_TRUE(err);
        EXPECT_EQ(err.value(), "context cancelled");
    }
}