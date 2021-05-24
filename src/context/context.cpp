#include "context.hpp"
#include <cassert>

namespace context {
Token::Token(std::stop_token&& token) : signal(std::make_shared<std::stop_token>(std::move(token))) {}

bool Token::operator()() const {
    if(!signal) {
        return false;
    }
    if(!signal->stop_possible()) {
        return false;
    }
    return signal->stop_requested();
}

Context::Context(std::shared_ptr<Context> ctx) : parent(std::move(ctx)) {}

void Context::cancel() {
    if(parent != nullptr) {
        parent->cancel();
    }
}
}

