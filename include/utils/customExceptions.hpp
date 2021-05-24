#pragma once
#include <exception>
#include <string>

namespace context {
    class CustomException : public std::exception {
    protected:
        std::string errorString;

    public:
        CustomException() = default;
        explicit CustomException(std::string&& error_p);
        [[nodiscard]] const char* what() const noexcept override;
    };

}