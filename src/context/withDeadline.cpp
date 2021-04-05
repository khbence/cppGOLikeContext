#include "withDeadline.hpp"
#include <thread>

namespace context{
DeadlineExceeded::DeadlineExceeded() : CustomException("deadline exceeded") {}

WithDeadline::WithDeadline(time deadlineP, std::shared_ptr<Context>&& parentP)
                : WithCancel(std::move(parentP))
                , deadlineVar(deadlineP) {
    std::thread th{[this](){
        std::this_thread::sleep_until(deadlineVar);
        this->cancelWithError(std::make_unique<DeadlineExceeded>());
    }};
    th.detach();
}

std::optional<time> WithDeadline::deadline() {
    return deadlineVar;
}
}