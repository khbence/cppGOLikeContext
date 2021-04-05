#include "contextFactory.hpp"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

using namespace std::literals::chrono_literals;

void printingSlowly(const std::shared_ptr<context::Context>& ctx) {
    constexpr unsigned steps = 10;
    auto done = ctx->done();
    for(unsigned i = 0; i < steps; ++i) {
        if(done->stop_requested()) {
            break;
        }
        std::cout << "steps: " << i << std::endl;
        std::this_thread::sleep_for(1s);
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
    auto ctx = context::ContextFactory::createWithCancelContext(context::ContextFactory::createBackgroundContext());
    [[maybe_unused]] auto now = std::chrono::system_clock::now();
    ctx = context::ContextFactory::createWithTimeoutContext(2500ms, ctx);
    std::jthread th{printingSlowly, ctx};
    std::this_thread::sleep_for(1500ms);
    ctx->cancel();
    std::cout << ctx->err()->what() << std::endl;
    return EXIT_SUCCESS;
}
