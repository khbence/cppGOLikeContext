#include "createContext.hpp"
#include <iostream>
#include <memory>
#include <thread>

using namespace std::literals::chrono_literals;

void printingSlowly(std::shared_ptr<context::Context> ctx) {
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
  auto [ctx, cancel] = context::createWithCancelContext(context::createBackgroundContext());
  std::jthread th{printingSlowly, ctx};
  std::this_thread::sleep_for(2500ms);
  cancel();
  return EXIT_SUCCESS;
}
