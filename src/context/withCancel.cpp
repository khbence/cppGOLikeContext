#include "withCancel.hpp"

namespace context {
Canceled::Canceled() : CustomException("context canceled") {}

WithCancel::WithCancel(std::shared_ptr<Context>&& parentP) : Context(std::move(parentP)) {}

std::optional<time> WithCancel::deadline() {
    return parent->deadline();
}

doneSignal WithCancel::done() {
    return cancelToken.get_token();
}

std::exception* WithCancel::err() {
    std::lock_guard<std::mutex> guard{errorLocker};
    return error.get();
}

const std::any& WithCancel::value(const std::any& key) {
    return parent->value(key);
}


void WithCancel::cancel() {
    std::lock_guard<std::mutex> locker{errorLocker};
    if(cancelToken.stop_requested()) {
        return;
    }
    cancelToken.request_stop();
    error = std::make_unique<Canceled>();
    if(parent != nullptr) {
        parent->cancel();
    }
}
} // namespace context