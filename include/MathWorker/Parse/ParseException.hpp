#pragma once

#include "../MathUtils/MathDefines.hpp"

namespace mathWorker
{
	enum class ExceptionType : unsigned char
	{
		unknown = 0,
		brackets,
		params,
		priority
	};

	class ParseException : public std::exception
	{
	public:
		explicit ParseException(const char* message, ExceptionType type);
		explicit ParseException(const std::string& message, ExceptionType type);
		~ParseException() noexcept override;

		const char* what() const noexcept override;
		ExceptionType type() const noexcept;

	protected:

		std::string msg_;

		ExceptionType type_ = ExceptionType::unknown;

	};
}
