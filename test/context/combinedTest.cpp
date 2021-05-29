#include "gtest/gtest.h"
#include "contextFactory.hpp"
#include <chrono>
#include <thread>
#include <tuple>
#include <array>
#include <mutex>

using namespace std::literals::chrono_literals;

class CombinedTest : public ::testing::Test {
protected:
    typedef std::tuple<std::any, std::any, const std::type_info&, const std::type_info&>
        keyValueData;
    typedef std::pair<int, double> examplePair;
    const std::type_info& INT = typeid(int);
    const std::type_info& STRING = typeid(std::string);
    const std::type_info& CHAR = typeid(char);
    const std::type_info& PAIR = typeid(examplePair);

    static constexpr unsigned N = 7;
    static inline std::optional<std::string> cancelled = "context cancelled";
    static inline std::optional<std::string> exceeded = "deadline exceeded";
    static inline std::optional<std::string> noError;

    context::time wholeTimeOut = std::chrono::system_clock::now() + 3s;
    context::time deadline1 = std::chrono::system_clock::now() + 2s;
    context::time deadline2 = std::chrono::system_clock::now() + 1s;
    keyValueData v1{ 1, 'a', INT, CHAR };
    keyValueData v2{ std::string{ "A" }, examplePair{ 2, 33.0 }, STRING, PAIR };

    std::array<std::jthread, N> threads;
    std::array<std::shared_ptr<context::Context>, N> contexts;
    std::array<std::atomic<bool>, N> dones;
    std::array<std::optional<std::string>, N> errs;
    std::mutex errorMutex;

    void job(std::shared_ptr<context::Context> ctx, unsigned idx) {
        dones.at(idx).store(ctx->done());
        errs.at(idx) = ctx->err();
        while ((!ctx->done()) && std::chrono::system_clock::now() < wholeTimeOut) {
            std::this_thread::sleep_for(0.2s);
        }
        dones.at(idx).store(ctx->done());
        errorMutex.lock();
        errs.at(idx) = ctx->err();
        errorMutex.unlock();
    }

    void SetUp() override {
        unsigned i = 0;
        // back, cancel, deadline1, v1, cancel, v2, deadline2
        auto ctx = context::ContextFactory::createBackgroundContext();
        contexts.at(i++) = ctx;
        ctx = context::ContextFactory::createWithCancelContext(ctx);
        contexts.at(i++) = ctx;
        ctx = context::ContextFactory::createWithDeadlineContext(deadline1, ctx);
        contexts.at(i++) = ctx;
        ctx = context::ContextFactory::createWithValueContext(
            std::any{ std::get<0>(v1) }, std::any{ std::get<1>(v1) }, ctx);
        contexts.at(i++) = ctx;
        ctx = context::ContextFactory::createWithCancelContext(ctx);
        contexts.at(i++) = ctx;
        ctx = context::ContextFactory::createWithValueContext(
            std::any{ std::get<0>(v2) }, std::any{ std::get<1>(v2) }, ctx);
        contexts.at(i++) = ctx;
        ctx = context::ContextFactory::createWithDeadlineContext(deadline2, ctx);
        contexts.at(i++) = ctx;
        for (unsigned i = 0; i < N; ++i) {
            threads.at(i) = std::jthread{ [this, i, ctx]() { job(contexts.at(i), i); } };
        }
    }

    void TearDown() {
        for (auto& t : threads) { t.join(); }
    }

    void checkAllValues() {
        auto val = contexts.back()->value(std::get<0>(v1));
        ASSERT_TRUE(val.has_value());
        ASSERT_NO_THROW(std::any_cast<char>(val));
        EXPECT_EQ(std::any_cast<char>(val), std::any_cast<char>(std::get<1>(v1)));
        val = contexts.back()->value(std::get<0>(v2));
        ASSERT_TRUE(val.has_value());
        ASSERT_NO_THROW(std::any_cast<examplePair>(val));
        EXPECT_EQ(std::any_cast<examplePair>(val), std::any_cast<examplePair>(std::get<1>(v2)));
    }

    void check(std::array<bool, N>& expectedDones,
        std::array<std::optional<std::string>, N> expectedErrs) {
        errorMutex.lock();
        for (unsigned i = 0; i < N; ++i) {
            EXPECT_EQ(expectedDones.at(i), dones.at(i).load());
            EXPECT_EQ(expectedErrs.at(i), errs.at(i));
        }
        errorMutex.unlock();
        checkAllValues();
    }

    template<typename V>
    void compareTwoValues(const std::any& refValue, const std::any& val) {
        if (refValue.has_value()) {
            ASSERT_TRUE(val.has_value());
            ASSERT_NO_THROW(std::any_cast<V>(val));
            EXPECT_EQ(std::any_cast<V>(refValue), std::any_cast<V>(val));
        } else {
            EXPECT_FALSE(val.has_value());
        }
    }
};

TEST_F(CombinedTest, cancelAll) {
    std::array<bool, N> expectedDones{ false, true, true, true, true, true, true };
    std::array<std::optional<std::string>, N> expectedErrs{
        noError, cancelled, cancelled, cancelled, cancelled, cancelled, cancelled
    };
    contexts.at(1)->cancel();
    std::this_thread::sleep_until(wholeTimeOut + 0.5s);
    check(expectedDones, expectedErrs);
}

TEST_F(CombinedTest, timeOutMultiple) {
    // first deadline
    std::array<bool, N> expectedDones{ false, false, false, false, false, false, true };
    std::array<std::optional<std::string>, N> expectedErrs{
        noError, noError, noError, noError, noError, noError, exceeded
    };
    std::this_thread::sleep_for(1.4s);
    check(expectedDones, expectedErrs);

    // cancel lower
    expectedDones = std::array<bool, N>{ false, false, false, false, true, true, true };
    expectedErrs = std::array<std::optional<std::string>, N>{
        noError, noError, noError, noError, cancelled, cancelled, exceeded
    };
    contexts.at(4)->cancel();
    std::this_thread::sleep_for(0.4s);
    check(expectedDones, expectedErrs);

    // second deadline
    expectedDones = std::array<bool, N>{ false, false, true, true, true, true, true };
    expectedErrs = std::array<std::optional<std::string>, N>{
        noError, noError, exceeded, exceeded, cancelled, cancelled, exceeded
    };
    std::this_thread::sleep_for(0.4s);
    check(expectedDones, expectedErrs);

    // cancel upper
    expectedDones = std::array<bool, N>{ false, true, true, true, true, true, true };
    expectedErrs = std::array<std::optional<std::string>, N>{
        noError, cancelled, exceeded, exceeded, cancelled, cancelled, exceeded
    };
    contexts.at(1)->cancel();
    std::this_thread::sleep_until(wholeTimeOut + 0.5s);
    check(expectedDones, expectedErrs);
}

TEST_F(CombinedTest, cancelNonWithCancel) {
    // first deadline
    std::array<bool, N> expectedDones{ false, false, false, false, false, false, true };
    std::array<std::optional<std::string>, N> expectedErrs{
        noError, noError, noError, noError, noError, noError, exceeded
    };
    std::this_thread::sleep_for(1.4s);
    check(expectedDones, expectedErrs);
}

TEST_F(CombinedTest, findValues) {
    for (unsigned i = 0; i < 3; ++i) {
        compareTwoValues<char>(std::any{}, contexts.at(i)->value(std::get<0>(v1)));
        compareTwoValues<examplePair>(std::any{}, contexts.at(i)->value(std::get<0>(v2)));
    }
    for (unsigned i = 3; i < 5; ++i) {
        compareTwoValues<char>(std::get<1>(v1), contexts.at(i)->value(std::get<0>(v1)));
        compareTwoValues<examplePair>(std::any{}, contexts.at(i)->value(std::get<0>(v2)));
    }
    for (unsigned i = 5; i < 6; ++i) {
        compareTwoValues<char>(std::get<1>(v1), contexts.at(i)->value(std::get<0>(v1)));
        compareTwoValues<examplePair>(std::get<1>(v2), contexts.at(i)->value(std::get<0>(v2)));
    }
}