#pragma once
#include "background.hpp"
#include <any>
#include "customExceptions.hpp"

namespace context {
class EmptyKeyException : public CustomException {
public:
    EmptyKeyException();
};

class WithValue : public Background {
    std::any keyVar;
    std::any valueVar;

public:
    WithValue(std::any keyP, std::any valueP, std::shared_ptr<Context> parentP);
    [[nodiscard]] const std::any& value(const std::any& key) const override;
};
}// namespace context