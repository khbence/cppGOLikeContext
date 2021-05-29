#include "customExceptions.hpp"

namespace context {
CustomException::CustomException(std::string&& error_p) : errorString(std::move(error_p)) {}

const char* CustomException::what() const noexcept { return errorString.c_str(); }
}// namespace context