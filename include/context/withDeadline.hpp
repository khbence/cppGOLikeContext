#pragma once
#include "withCancel.hpp"
#include <chrono>

namespace context {
class DeadlineExceeded : public CustomException {
public:
    DeadlineExceeded();
};

class WithDeadline : public WithCancel {
    time deadlineVar;
public:
    explicit WithDeadline(time deadlineP, std::shared_ptr<Context>&& parentP);

    [[nodiscard]] std::optional<time> deadline() override;
};
}