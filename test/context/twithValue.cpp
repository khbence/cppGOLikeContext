#include "gtest/gtest.h"
#include "contextFactory.hpp"
#include <vector>
#include <tuple>
#include "withValue.hpp"

auto& INT = typeid(int);
auto& STRING = typeid(std::string);
auto& CHAR = typeid(char);
auto& PAIR = typeid(std::pair<int, double>);

TEST(withValue, allFunctionDefault) {
    auto ctx = context::ContextFactory::createWithValueContext(
        1, 2, context::ContextFactory::createBackgroundContext());
    EXPECT_FALSE(ctx->deadline());
    EXPECT_FALSE(ctx->done());
    EXPECT_FALSE(ctx->err());
    ASSERT_TRUE(ctx->value(1).has_value());
    auto val = ctx->value(1);
    ASSERT_NO_THROW(std::any_cast<int>(val));
    EXPECT_THROW(std::any_cast<std::string>(val), std::bad_any_cast);
    EXPECT_THROW(std::any_cast<const char*>(val), std::bad_any_cast);
    EXPECT_EQ(std::any_cast<int>(val), 2);
}

TEST(withValue, emptyKey) {
    EXPECT_THROW(auto ctx = context::ContextFactory::createWithValueContext(
                     std::any{}, 2, context::ContextFactory::createBackgroundContext()),
        context::EmptyKeyException);
}

template<typename V>
void checkValue(const std::any& refValue, const std::any& val) {
    ASSERT_TRUE(val.has_value());
    ASSERT_NO_THROW(std::any_cast<V>(val));
    EXPECT_EQ(std::any_cast<V>(refValue), std::any_cast<V>(val));
}

TEST(withValue, singleValueAll) {
    std::vector<std::tuple<std::any, std::any, const std::type_info&, const std::type_info&>>
        testCases{ { 1, 2, INT, INT },
            { 'a', 'b', CHAR, CHAR },
            { std::string("A"), std::string("B"), STRING, STRING },
            { 1, 'n', INT, CHAR },
            { std::string("K"), 4, STRING, INT },
            { 'v', std::pair<int, double>{ 2, 33.0 }, CHAR, PAIR } };
    for (const auto& [key, refValue, keyType, valueType] : testCases) {
        auto ctx = context::ContextFactory::createWithValueContext(
            std::any(key), std::any(refValue), context::ContextFactory::createBackgroundContext());
        auto val = ctx->value(key);
        if (valueType == INT) {
            checkValue<int>(refValue, val);
        } else if (valueType == STRING) {
            checkValue<std::string>(refValue, val);
        } else if (valueType == CHAR) {
            checkValue<char>(refValue, val);
        } else if (valueType == PAIR) {
            checkValue<std::pair<int, double>>(refValue, val);
        }
    }
}

TEST(withValue, nestedValues) {
    std::vector<std::tuple<std::any, std::any, const std::type_info&, const std::type_info&>>
        testCases{ { 1, 2, INT, INT },
            { 'a', 'b', CHAR, CHAR },
            { std::string("A"), std::string("B"), STRING, STRING },
            { 2, 'n', INT, CHAR },
            { std::string("K"), 4, STRING, INT },
            { 'v', std::pair<int, double>{ 2, 33.0 }, CHAR, PAIR } };
    auto ctx = context::ContextFactory::createWithCancelContext(
        context::ContextFactory::createBackgroundContext());
    for ([[maybe_unused]] const auto& [key, refValue, keyType, valueType] : testCases) {
        ctx = context::ContextFactory::createWithValueContext(
            std::any{ key }, std::any{ refValue }, ctx);
    }

    for (int i = 0; const auto& [key, refValue, keyType, valueType] : testCases) {
        auto val = ctx->value(key);
        if (valueType == INT) {
            checkValue<int>(refValue, val);
        } else if (valueType == STRING) {
            checkValue<std::string>(refValue, val);
        } else if (valueType == CHAR) {
            checkValue<char>(refValue, val);
        } else if (valueType == PAIR) {
            checkValue<std::pair<int, double>>(refValue, val);
        }
    }
}