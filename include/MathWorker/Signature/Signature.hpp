#pragma once

#include "../MathUtils/MathDefines.hpp"

namespace mathWorker
{

	using NativeRealization = std::function<MathNodeP(const std::vector<MathNodeP>&)>;
	using MatherRealization = std::pair<MathNodeP, std::vector<std::string>>;

	enum OperatorPriority : unsigned char
	{
		none = 0,
		leftToRight,
		rightToLeft
	};

	struct SignatureRealization
	{
		std::variant<NativeRealization, MatherRealization> realization;
		unsigned char priority = 0;
		SignatureType type = SignatureType::operation;
		OperatorPriority assitiation = OperatorPriority::none;
	};

	using SignatureContext = std::map<std::string, SignatureRealization, std::less<>>;


	class Signature
	{
	public:
		Signature() = default;
		
		void addFunction(const std::string& name, std::variant<NativeRealization, MatherRealization> realization)
		{
			context_[name] = { std::move(realization), 0, SignatureType::function };
		}
		void addSpecialFunction(const std::string& name, std::variant<NativeRealization, MatherRealization> realization)
		{
			context_[name] = { std::move(realization), 0, SignatureType::specialFunction };
		}
		void addUnareRightOperator(const std::string& name, NativeRealization realization)
		{
			context_[name] = { realization, 1, SignatureType::unareRight };
		}
		void addUnareLeftOperator(const std::string& name, NativeRealization realization)
		{
			context_[name] = { realization, 2, SignatureType::unareLeft };
		}
		void addOperator(const std::string& name, NativeRealization realization, const unsigned char priority, const OperatorPriority direction = OperatorPriority::leftToRight)
		{
			context_[name] = { realization, static_cast<unsigned char>(priority + 3), SignatureType::operation, direction };
		}

		void setDefaultOperator(const std::string& defautlOperator)
		{
			if (isTerm(defautlOperator))
				defaultOperator_ = defautlOperator;
		}
		const std::string& getDefaultOperator() const
		{
			return defaultOperator_;
		}

		bool isTerm(const std::string& str) const
		{
			return context_.find(str) != context_.end();
		}
		bool isTerm(const std::string_view str) const
		{
			return context_.find(str) != context_.end();
		}

		bool isSignatureType(const std::string& str, const SignatureType type) const
		{
			const auto& found = context_.find(str);
			return found == context_.end() ? false : (found->second.type == type);
		}
		bool isSignatureType(const std::string_view str, const SignatureType type) const
		{
			const auto& found = context_.find(str);
			return found == context_.end() ? false : (found->second.type == type);
		}

		const SignatureRealization* operator[](const std::string& str) const
		{
			return at(str);
		}
		const SignatureRealization* operator[](const std::string_view str) const
		{
			return at(str);
		}
		SignatureRealization* operator[](const std::string& str)
		{
			return at(str);
		}
		SignatureRealization* operator[](const std::string_view str)
		{
			return at(str);
		}
		
		const SignatureRealization* at(const std::string& str) const
		{
			auto found = context_.find(str);
			return (found == context_.end()) ? (nullptr) : (&found->second);
		}
		const SignatureRealization* at(const std::string_view str) const
		{
			auto found = context_.find(str);
			return (found == context_.end()) ? (nullptr) : (&found->second);
		}
		SignatureRealization* at(const std::string& str)
		{
			auto found = context_.find(str);
			return (found == context_.end()) ? (nullptr) : (&found->second);
		}
		SignatureRealization* at(const std::string_view str)
		{
			auto found = context_.find(str);
			return (found == context_.end()) ? (nullptr) : (&found->second);
		}



	private:

		SignatureContext context_;

		std::string defaultOperator_;

	};


}