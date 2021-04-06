#pragma once
#include "context.hpp"

namespace context {
    class Background : public Context {
        static std::any emptyAny;
    protected:
        explicit Background(std::shared_ptr<Context>&& parentP);
    public:
        Background() = default;
        [[nodiscard]] std::optional<time> deadline() override;
        [[nodiscard]] doneSignal done() override;
        [[nodiscard]] std::exception* err() override;
        [[nodiscard]] const std::any& value(const std::any& key) override;
    };
}