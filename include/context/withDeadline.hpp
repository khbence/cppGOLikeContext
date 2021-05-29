#pragma once
#include "withCancel.hpp"
#include <chrono>
#include <mutex>
#include <condition_variable>

namespace context {
class DeadlineExceeded : public CustomException {
public:
    DeadlineExceeded();
};

class WithDeadline : public WithCancel {
    time deadlineVar;
    std::atomic<bool> cancelledTheWait{false};
    std::atomic<bool> finishedTheCancellation{false};
    std::mutex waitMutex;
    std::condition_variable cv;
    std::jthread waitingThread;
public:
    explicit WithDeadline(time deadlineP, std::shared_ptr<Context> parentP);
    ~WithDeadline();

    [[nodiscard]] std::optional<time> deadline() const override;
    void cancel() override;
};
}