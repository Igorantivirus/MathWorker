#include "MathWorker/Parse/ParseException.hpp"

namespace mathWorker
{

	ParseException::ParseException(const char* message, ExceptionType type)
		: msg_(message), type_{ type }
	{
	}

	ParseException::ParseException(const std::string& message, ExceptionType type)
		: msg_(message), type_{ type }
	{
	}

	ParseException::~ParseException() noexcept = default;

	const char* ParseException::what() const noexcept
	{
		return msg_.c_str();
	}

	ExceptionType ParseException::type() const noexcept
	{
		return type_;
	}

}
