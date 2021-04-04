#include <iostream>
#include <any>
#include "createContext.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char const* argv[]) {
    auto ctx = context::createBackgroundContext();
    auto tmp = ctx->value(std::any{});
    std::cout << tmp.has_value() << std::endl;
    return EXIT_SUCCESS;
}
