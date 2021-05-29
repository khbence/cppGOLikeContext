#include "context.hpp"
#include <cassert>

namespace context {
Context::Context(std::shared_ptr<Context> ctx) : parent(std::move(ctx)) {}

void Context::cancel() {
    if (parent != nullptr) { parent->cancel(); }
}
}// namespace context
