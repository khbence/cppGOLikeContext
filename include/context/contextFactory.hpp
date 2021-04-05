#pragma once
#include "context.hpp"
#include <memory>

namespace context {
    class ContextFactory {
    public:
        [[nodiscard]] static std::shared_ptr<Context> createBackgroundContext();
        [[nodiscard]] static std::shared_ptr<Context> createTODOContext();
        [[nodiscard]] static std::shared_ptr<Context> createWithCancelContext(std::shared_ptr<Context> parent);
        [[nodiscard]] static std::shared_ptr<Context> createWithDeadlineContext(time deadline, std::shared_ptr<Context> parent);
    };

}