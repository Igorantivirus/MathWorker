#include "MathWorker/StringConvert/ConvertStringException.hpp"

namespace mathWorker
{

	ConvertStringException::ConvertStringException(const char* message)
		: msg_(message)
	{
	}

	ConvertStringException::ConvertStringException(const std::string& message)
		: msg_(message)
	{
	}

	ConvertStringException::~ConvertStringException() noexcept = default;

	const char* ConvertStringException::what() const noexcept
	{
		return msg_.c_str();
	}

}
