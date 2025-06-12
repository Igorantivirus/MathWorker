#pragma once

#include <string>
#include <exception>

namespace mathWorker
{

	class ConvertStringException : public std::exception
	{
	public:
		explicit ConvertStringException(const char* message)
			: msg_(message)
		{}
		explicit ConvertStringException(const std::string& message)
			: msg_(message)
		{}
		virtual ~ConvertStringException() noexcept
		{}

		virtual const char* what() const noexcept
		{
			return msg_.c_str();
		}

	protected:

		std::string msg_;

	};

}