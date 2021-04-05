#include "createContext.hpp"
#include "background.hpp"
#include "todo.hpp"
#include "withCancel.hpp"

namespace context {
std::shared_ptr<Context> createBackgroundContext() {
  return std::make_shared<Background>();
}

std::shared_ptr<Context> createTODOContext() {
  return std::make_shared<TODO>();
}

std::pair<std::shared_ptr<Context>, std::function<void()>> createWithCancelContext(std::shared_ptr<Context>&& ctx) {
    auto newCtx = std::make_shared<WithCancel>(std::move(ctx));
    auto cancel = [newCtx]() {
        std::lock_guard<std::mutex> locker{newCtx->errorLocker};
        newCtx->cancelToken.request_stop();
        newCtx->error = std::make_unique<Canceled>();
    };
    return std::make_pair(std::move(newCtx), cancel);
}
} // namespace context