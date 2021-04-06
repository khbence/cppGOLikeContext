#include "withValue.hpp"

namespace context {
    EmptyKeyException::EmptyKeyException() : CustomException("empty key for WithValue initialisation is not allowed") {}

    WithValue::WithValue(std::any&& keyP, std::any&& valueP, std::shared_ptr<Context>&& parent)
                            : Background(std::move(parent))
                            , keyVar(std::move(keyP))
                            , valueVar(std::move(valueP)) {
        if (!keyVar.has_value()) {
            throw EmptyKeyException();
        }
    }

    const std::any& WithValue::value(const std::any& key) {
        if(key.type() == keyVar.type()) {
            const auto& type = key.type();
            if(type == typeid(int)) {
                if(std::any_cast<int>(key) == std::any_cast<int>(keyVar)) {
                    return valueVar;
                }
            } else if(type == typeid(std::string)) {
                if(std::any_cast<std::string&>(key) == std::any_cast<std::string&>(keyVar)) {
                    return valueVar;
                }
            } else if(type == typeid(char)) {
                if(std::any_cast<char>(key) == std::any_cast<char>(keyVar)) {
                    return valueVar;
                }
            }
        }
        return parent->value(key);
    }
}