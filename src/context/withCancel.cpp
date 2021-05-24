#include "withCancel.hpp"

namespace context {
Canceled::Canceled() : CustomException("context canceled") {}

void WithCancel::cancelWithError(std::unique_ptr<CustomException>&& errorP) {
    std::lock_guard<std::mutex> locker{errorLocker};
    if(isCanceled.load()) {
        return;
    }
    isCanceled.store(true);
    error = std::move(errorP);
}

WithCancel::WithCancel(std::shared_ptr<Context>&& parentP) : Background(std::move(parentP)) {}

doneSignal WithCancel::done() {
    return isCanceled.load() || parent->done();
}

std::exception* WithCancel::err() {
    std::lock_guard<std::mutex> guard{errorLocker};
    return error.get();
}

void WithCancel::cancel() {
    cancelWithError(std::make_unique<Canceled>());
}
} // namespace context