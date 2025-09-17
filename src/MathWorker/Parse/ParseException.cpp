#include "ParseException.hpp"

namespace mathWorker
{


    explicit ParseException::ParseException(const char* message, const ExceptionType type)
        : msg_(message), type_{ type }
    {}
    explicit ParseException::ParseException(const std::string& message, const ExceptionType type)
        : msg_(message), type_{ type }
    {}
    ParseException::~ParseException() noexcept
    {}

    const char* ParseException::what() const noexcept
    {
        return msg_.c_str();
    }
    const ExceptionType ParseException::type() const noexcept
    {
        return type_;
    }
}