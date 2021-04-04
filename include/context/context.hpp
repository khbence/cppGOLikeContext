#pragma once
#include <optional>
#include <exception>
#include <any>
#include <memory>

namespace context {
    // TODO create some actual useful type for it
    typedef int time;
    typedef bool channel;

    class Context {
    public:
        virtual ~Context() = default;

        [[nodiscard]] virtual std::optional<time> deadline() = 0;
        [[nodiscard]] virtual std::shared_ptr<channel> done() = 0;
        [[nodiscard]] virtual std::unique_ptr<std::exception> err() = 0;
        [[nodiscard]] virtual const std::any& value(const std::any& key) = 0;
    };
}