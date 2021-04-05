#pragma once
#include "context.hpp"
#include <memory>
#include <functional>
#include <tuple>

namespace context {
    [[nodiscard]] std::shared_ptr<Context> createBackgroundContext();
    [[nodiscard]] std::shared_ptr<Context> createTODOContext();
    [[nodiscard]] std::pair<std::shared_ptr<Context>, std::function<void()>> createWithCancelContext(std::shared_ptr<Context>&& ctx);
}