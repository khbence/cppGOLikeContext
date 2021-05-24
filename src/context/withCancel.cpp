#include "withCancel.hpp"

namespace context {
Canceled::Canceled() : CustomException("context canceled") {}

void WithCancel::cancelWithError(std::unique_ptr<CustomException>&& errorP) {
    std::lock_guard<std::mutex> locker{errorLocker};
    if(cancelToken.stop_requested()) {
        return;
    }
    cancelToken.request_stop();
    error = std::move(errorP);
    if(parent != nullptr) {
        parent->cancel();
    }
}

WithCancel::WithCancel(std::shared_ptr<Context>&& parentP) : Background(std::move(parentP)) {}

doneSignal WithCancel::done() {
    return Token{std::move(cancelToken.get_token())};
}

std::exception* WithCancel::err() {
    std::lock_guard<std::mutex> guard{errorLocker};
    return error.get();
}

void WithCancel::cancel() {
    cancelWithError(std::make_unique<Canceled>());
}
} // namespace context