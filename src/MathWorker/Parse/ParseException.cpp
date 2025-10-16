#include <MathWorker/Parse/ParseException.hpp>

namespace mathWorker
{


    ParseException::ParseException(const char* message, const ExceptionType type)
        : msg_(message), type_{ type }
    {}
    ParseException::ParseException(const std::string& message, const ExceptionType type)
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