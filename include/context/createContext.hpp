#pragma once
#include "context.hpp"
#include <memory>

namespace context {
    [[nodiscard]] std::shared_ptr<Context> createBackgroundContext();
    [[nodiscard]] std::shared_ptr<Context> createTODOContext();
    [[nodiscard]] std::shared_ptr<Context> createWithCancelContext(std::shared_ptr<Context> ctx);
}