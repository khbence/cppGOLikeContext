#include "background.hpp"

namespace context {
    std::any Background::emptyAny;

    std::optional<time> Background::deadline() {
        return std::optional<time>();
    }

    std::shared_ptr<channel> Background::done() {
        return nullptr;
    }

    std::unique_ptr<std::exception> Background::err() {
        return nullptr;
    }

    const std::any& Background::value([[maybe_unused]] const std::any &key) {
        return Background::emptyAny;
    }
}