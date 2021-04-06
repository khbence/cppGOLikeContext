#include "contextFactory.hpp"
#include "background.hpp"
#include "todo.hpp"
#include "withCancel.hpp"
#include "withDeadline.hpp"
#include "withValue.hpp"

namespace context {
std::shared_ptr<Context> ContextFactory::createBackgroundContext() {
    return std::make_shared<Background>();
}

std::shared_ptr<Context> ContextFactory::createTODOContext() {
    return std::make_shared<TODO>();
}

std::shared_ptr<Context> ContextFactory::createWithCancelContext(std::shared_ptr<Context> parent) {
    return std::make_shared<WithCancel>(std::move(parent));
}

std::shared_ptr<Context> ContextFactory::createWithValueContext(std::any&& key, std::any&& value, std::shared_ptr<Context> parent) {
    return std::make_shared<WithValue>(std::move(key), std::move(value), std::move(parent));
}

std::shared_ptr<Context> ContextFactory::createWithDeadlineContext(time deadline, std::shared_ptr<Context> parent) {
    return std::make_shared<WithDeadline>(deadline, std::move(parent));
}
} // namespace context