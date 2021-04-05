#include "context.hpp"

namespace context {
Context::Context(std::shared_ptr<Context> ctx) : parent(std::move(ctx)) {}
}

