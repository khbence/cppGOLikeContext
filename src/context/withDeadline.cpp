#include "withDeadline.hpp"
#include <thread>

namespace context{
DeadlineExceeded::DeadlineExceeded() : CustomException("deadline exceeded") {}

WithDeadline::WithDeadline(time deadlineP, std::shared_ptr<Context>&& parentP)
                : WithCancel(std::move(parentP))
                , deadlineVar(deadlineP) {
    std::thread th{[this](){
        std::unique_lock<std::mutex> ul{waitMutex};
        if(cv.wait_until(ul, deadlineVar, [this](){return canceledTheWait.load();})) {
            cancelWithError(std::make_unique<Canceled>());
        } else {
            cancelWithError(std::make_unique<DeadlineExceeded>());
            canceledTheWait.store(false);
            cv.notify_all();
        }
    }};
    th.detach();
}

std::optional<time> WithDeadline::deadline() {
    return deadlineVar;
}

void WithDeadline::cancel() {
    if(err() != nullptr) {
        return;
    }
    canceledTheWait.store(true);
    cv.notify_all();
    std::unique_lock<std::mutex> ul{waitMutex};
    cv.wait(ul, [this]() { return !canceledTheWait.load();});
    canceledTheWait.store(true);
}
}