#include "withCancel.hpp"

namespace context {
Cancelled::Cancelled() : CustomException("context cancelled") {}

void WithCancel::cancelWithError(std::unique_ptr<CustomException> errorP) {
    std::lock_guard<std::mutex> locker{ errorLocker };
    if (isCancelled.load()) { return; }
    isCancelled.store(true);
    error = std::move(errorP);
}

WithCancel::WithCancel(std::shared_ptr<Context> parentP) : Background(std::move(parentP)) {}

bool WithCancel::done() const {
    if (parent) { return isCancelled.load() || parent->done(); }
    return isCancelled.load();
}

std::optional<std::string> WithCancel::err() const {
    {
        std::lock_guard<std::mutex> guard{ errorLocker };
        if (error) { return std::string(error->what()); }
    }
    if (parent) { return parent->err(); }
    return {};
}

void WithCancel::cancel() { cancelWithError(std::make_unique<Cancelled>()); }
}// namespace context