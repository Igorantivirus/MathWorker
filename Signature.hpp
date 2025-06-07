#pragma once

#include "MathDefines.hpp"

namespace mathWorker
{

	class Signature
	{
	public:
		Signature() = default;

		void addFunction(const std::string& name, NativeRealization realization)
		{
			context_[name] = { realization,0, SignatureType::function };
		}
		void addUnareOperator(const std::string& name, NativeRealization realization)
		{
			context_[name] = { realization, 1, SignatureType::operation };
		}
		void addOperator(const std::string& name, NativeRealization realization, const unsigned char priority, const OperatorPriority direction = OperatorPriority::leftToRight)
		{
			context_[name] = { realization, static_cast<unsigned char>(priority + 2), SignatureType::operation, direction };
		}

		bool isTerm(const std::string& str) const
		{
			return context_.find(str) != context_.end();
		}

		const SignatureRealization* operator[](const std::string& str) const
		{
			auto found = context_.find(str);
			return (found == context_.end()) ? (nullptr) : (&found->second);
		}
		SignatureRealization* operator[](const std::string& str)
		{
			auto found = context_.find(str);
			return (found == context_.end()) ? (nullptr) : (&found->second);
		}

	private:

		SignatureContext context_;

	};


}