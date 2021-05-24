#include "gtest/gtest.h"
#include "customExceptions.hpp"
#include <string>

TEST(customException, emptyConstructor) {
    context::CustomException e;
    EXPECT_EQ(std::string{e.what()}, "");
}

TEST(customException, stringConstructor) {
    context::CustomException e{"test"};
    EXPECT_EQ(std::string{e.what()}, "test");
}

TEST(customException, catchable) {
    try {
        throw context::CustomException{"test"};
    } catch(const std::exception& e) {
        EXPECT_EQ(std::string{e.what()}, "test");
    }
}