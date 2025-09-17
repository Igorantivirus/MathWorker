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
		explicit ParseException(const char* message, const ExceptionType type);
		explicit ParseException(const std::string& message, const ExceptionType type);
		virtual ~ParseException() noexcept;

		virtual const char* what() const noexcept;
		virtual const ExceptionType type() const noexcept;

	protected:

		std::string msg_;

		ExceptionType type_ = ExceptionType::unknown;

	};
}