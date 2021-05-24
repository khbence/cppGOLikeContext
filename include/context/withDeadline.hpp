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
    std::atomic<bool> canceledTheWait = false;
    std::mutex waitMutex;
    std::condition_variable cv;
public:
    explicit WithDeadline(time deadlineP, std::shared_ptr<Context>&& parentP);

    [[nodiscard]] std::optional<time> deadline() override;
    void cancel() override;
};
}