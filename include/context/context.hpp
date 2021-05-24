#pragma once
#include <optional>
#include <exception>
#include <any>
#include <thread>
#include <memory>

namespace context {
    class Token {
        std::shared_ptr<std::stop_token> signal;

    public:
        Token() = default;
        explicit Token(std::stop_token&& token);
        [[nodiscard]] bool operator()() const;
    };

    // TODO create some actual useful type for it
    typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> time;
    typedef bool doneSignal;

    class Context {
    protected:
        std::shared_ptr<Context> parent;
        Context() = default;
        explicit Context(std::shared_ptr<Context> ctx);
    public:
        virtual ~Context() = default;

        [[nodiscard]] virtual std::optional<time> deadline() = 0;
        [[nodiscard]] virtual doneSignal done() = 0;
        [[nodiscard]] virtual std::exception* err() = 0;
        [[nodiscard]] virtual const std::any& value(const std::any& key) = 0;
        virtual void cancel();
    };
}