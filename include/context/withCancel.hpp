#pragma once
#include <memory>
#include "background.hpp"
#include "customExceptions.hpp"
#include <thread>
#include <functional>
#include <mutex>
class ContextFactory;

namespace context {
    class Canceled : public CustomException {
    public:
        Canceled();
    };

    class WithCancel : public Background {
        std::unique_ptr<CustomException> error;
        std::stop_source cancelToken;
        std::mutex errorLocker;
    protected:
        void cancelWithError(std::unique_ptr<CustomException>&& errorP);
    public:
        explicit WithCancel(std::shared_ptr<Context>&& parentP);

        [[nodiscard]] doneSignal done() override;
        [[nodiscard]] std::exception* err() override;
        void cancel() override;
    };
}