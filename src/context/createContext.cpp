#include "createContext.hpp"
#include "background.hpp"
#include "todo.hpp"

namespace context {
    std::shared_ptr<Context> createBackgroundContext() {
        return std::make_shared<Background>();
    }

    std::shared_ptr<Context> createTODOContext() {
        return std::make_shared<TODO>();
    }

    std::shared_ptr<Context> createWithCancelContext(std::shared_ptr<Context> ctx) {
        return ctx;
    }
}