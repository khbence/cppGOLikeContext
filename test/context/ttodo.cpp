#include "gtest/gtest.h"
#include "contextFactory.hpp"

TEST(todo, allFunctionDefault) {
    auto ctx = context::ContextFactory::createTODOContext();
    EXPECT_FALSE(ctx->deadline());
    EXPECT_FALSE(ctx->done());
    EXPECT_FALSE(ctx->err());
    EXPECT_FALSE(ctx->value("text").has_value());
}