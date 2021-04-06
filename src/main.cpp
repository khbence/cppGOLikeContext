#include "contextFactory.hpp"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <string>

using namespace std::literals::chrono_literals;

void printingSlowly(const std::shared_ptr<context::Context>& ctx) {
    constexpr unsigned steps = 10;
    auto done = ctx->done();
    for(unsigned i = 0; i < steps; ++i) {
        if(done->stop_requested()) {
            break;
        }
        std::string value = "no value";
        const auto& rawValue = ctx->value(static_cast<int>(i));
        if(rawValue.has_value()) {
            try {
                value = std::any_cast<const char*>(rawValue);
            } catch (const std::bad_any_cast& e) {
                value = e.what();
            }
        }
        std::cout << "steps: " << i << " " << value << std::endl;
        std::this_thread::sleep_for(1s);
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
    auto ctx = context::ContextFactory::createWithCancelContext(context::ContextFactory::createBackgroundContext());
    [[maybe_unused]] auto now = std::chrono::system_clock::now();
    ctx = context::ContextFactory::createWithTimeoutContext(5500ms, ctx);
    ctx = context::ContextFactory::createWithValueContext(1, "one", ctx);
    ctx = context::ContextFactory::createWithValueContext(3, "three", ctx);
    ctx = context::ContextFactory::createWithValueContext(4, 4, ctx);
    std::jthread th{printingSlowly, ctx};
    std::this_thread::sleep_for(6500ms);
    ctx->cancel();
    std::cout << ctx->err()->what() << std::endl;
    return EXIT_SUCCESS;
}
