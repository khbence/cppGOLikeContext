#pragma once
#include "context.hpp"

namespace context {
    class Background : public Context {
        static std::any emptyAny;
    public:
        [[nodiscard]] std::optional<time> deadline() override;
        [[nodiscard]] std::shared_ptr<channel> done() override;
        [[nodiscard]] std::unique_ptr<std::exception> err() override;
        [[nodiscard]] const std::any& value(const std::any& key) override;
    };
}