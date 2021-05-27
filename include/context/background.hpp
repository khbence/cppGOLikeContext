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
        [[nodiscard]] bool done() override;
        [[nodiscard]] std::optional<std::string> err() override;
        [[nodiscard]] const std::any& value(const std::any& key) override;
    };
}