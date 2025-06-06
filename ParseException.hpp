#pragma once

#include "MathDefines.hpp"

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
		explicit ParseException(const char* message, const ExceptionType type)
			: msg_(message), type_{ type }
		{}
		explicit ParseException(const std::string& message, const ExceptionType type)
			: msg_(message), type_{ type }
		{}
		virtual ~ParseException() noexcept
		{}

		virtual const char* what() const noexcept
		{
			return msg_.c_str();
		}
		virtual const ExceptionType type() const noexcept
		{
			return type_;
		}

	protected:

		std::string msg_;

		ExceptionType type_ = ExceptionType::unknown;

	};
}