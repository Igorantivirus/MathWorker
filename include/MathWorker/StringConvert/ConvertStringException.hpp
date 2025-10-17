#pragma once

#include <string>
#include <exception>

namespace mathWorker
{

	class ConvertStringException : public std::exception
	{
	public:
		explicit ConvertStringException(const char* message);
		explicit ConvertStringException(const std::string& message);
		~ConvertStringException() noexcept override;

		const char* what() const noexcept override;

	protected:

		std::string msg_;

	};

}
