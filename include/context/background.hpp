#pragma once
#include "context.hpp"

namespace context {
class Background : public Context {
    static std::any emptyAny;

protected:
    explicit Background(std::shared_ptr<Context> parentP);

public:
    Background() = default;
    [[nodiscard]] std::optional<time> deadline() const override;
    [[nodiscard]] bool done() const override;
    [[nodiscard]] std::optional<std::string> err() const override;
    [[nodiscard]] const std::any& value(const std::any& key) const override;
};
}// namespace context