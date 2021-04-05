#include "createContext.hpp"
#include "background.hpp"
#include "todo.hpp"
#include "withCancel.hpp"

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
} // namespace context