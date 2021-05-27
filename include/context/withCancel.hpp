#pragma once
#include <memory>
#include "background.hpp"
#include "customExceptions.hpp"
#include <thread>
#include <functional>
#include <mutex>
#include <atomic>
class ContextFactory;

namespace context {
    class Cancelled : public CustomException {
    public:
        Cancelled();
    };

    class WithCancel : public Background {
        std::unique_ptr<CustomException> error;
        std::atomic_bool isCancelled;
        std::mutex errorLocker;
    protected:
        void cancelWithError(std::unique_ptr<CustomException>&& errorP);
    public:
        explicit WithCancel(std::shared_ptr<Context>&& parentP);

        [[nodiscard]] bool done() override;
        [[nodiscard]] std::optional<std::string> err() override;
        void cancel() override;
    };
}