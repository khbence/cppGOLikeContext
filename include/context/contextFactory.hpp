#pragma once
#include "context.hpp"
#include <memory>

namespace context {
    class ContextFactory {
    public:
        [[nodiscard]] static std::shared_ptr<Context> createBackgroundContext();
        [[nodiscard]] static std::shared_ptr<Context> createTODOContext();
        [[nodiscard]] static std::shared_ptr<Context> createWithCancelContext(std::shared_ptr<Context> parent);
        [[nodiscard]] static std::shared_ptr<Context> createWithValueContext(std::any key, std::any value, std::shared_ptr<Context> parent);

        [[nodiscard]] static std::shared_ptr<Context> createWithDeadlineContext(time deadline, std::shared_ptr<Context> parent);
        template<int64_t N>
        [[nodiscard]] static std::shared_ptr<Context> createWithTimeoutContext(std::chrono::duration<int64_t, std::ratio<1, N>> timeout, std::shared_ptr<Context> parent) {
            return createWithDeadlineContext(std::chrono::system_clock::now() + timeout, parent);
        }
    };

}