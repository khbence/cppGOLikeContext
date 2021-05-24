#include "withCancel.hpp"

namespace context {
Cancelled::Cancelled() : CustomException("context cancelled") {}

void WithCancel::cancelWithError(std::unique_ptr<CustomException>&& errorP) {
    std::lock_guard<std::mutex> locker{errorLocker};
    if(isCancelled.load()) {
        return;
    }
    isCancelled.store(true);
    error = std::move(errorP);
}

WithCancel::WithCancel(std::shared_ptr<Context>&& parentP) : Background(std::move(parentP)) {}

doneSignal WithCancel::done() {
    if(parent) {
        return isCancelled.load() || parent->done();
    }
    return isCancelled.load();
}

std::exception* WithCancel::err() {
    {
        std::lock_guard<std::mutex> guard{errorLocker};
        if(error) {
            return error.get();
        }
    }
    if(parent) {
        return parent->err();
    }
    return nullptr;
}

void WithCancel::cancel() {
    cancelWithError(std::make_unique<Cancelled>());
}
} // namespace context