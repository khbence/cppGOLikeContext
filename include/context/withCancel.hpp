#pragma once
#include <memory>
#include "background.hpp"
#include "customExceptions.hpp"
#include <thread>
#include <functional>
#include <mutex>

namespace context {
    class Canceled : public CustomException {
    public:
        Canceled();
    };

    class WithCancel : public Context {
        std::unique_ptr<CustomException> error;
        std::stop_source cancelToken;
        std::mutex errorLocker;

        friend std::pair<std::shared_ptr<Context>, std::function<void()>> createWithCancelContext(std::shared_ptr<Context>&& ctx);
    public:
        explicit WithCancel(std::shared_ptr<Context>&& parentP);

        [[nodiscard]] std::optional<time> deadline() override;
        [[nodiscard]] doneSignal done() override;
        [[nodiscard]] std::exception* err() override;
        [[nodiscard]] const std::any& value(const std::any& key) override;
    };
}