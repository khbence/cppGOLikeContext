#include "background.hpp"

namespace context {
std::any Background::emptyAny;

Background::Background(std::shared_ptr<Context>&& parentP) : Context(std::move(parentP)) {}

std::optional<time> Background::deadline() {
    if(parent == nullptr) {
        return {};
    }
    return parent->deadline();
}

doneSignal Background::done() {
    if(parent == nullptr) {
        return {};
    }
    return parent->done();
}

std::exception *Background::err() {
    if(parent == nullptr) {
        return nullptr;
    }
    return parent->err();
}

const std::any &Background::value([[maybe_unused]] const std::any &key) {
    if(parent == nullptr) {
        return emptyAny;
    }
    return parent->value(key);
}
} // namespace context