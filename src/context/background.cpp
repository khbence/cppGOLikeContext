#include "background.hpp"

namespace context {
std::any Background::emptyAny;

Background::Background(std::shared_ptr<Context> parentP) : Context(std::move(parentP)) {}

std::optional<time> Background::deadline() const {
    if(parent == nullptr) {
        return {};
    }
    return parent->deadline();
}

bool Background::done() const {
    if(parent == nullptr) {
        return false;
    }
    return parent->done();
}

std::optional<std::string> Background::err() const {
    if(parent == nullptr) {
        return {};
    }
    return parent->err();
}

const std::any &Background::value(const std::any &key) const {
    if(parent == nullptr) {
        return emptyAny;
    }
    return parent->value(key);
}
} // namespace context