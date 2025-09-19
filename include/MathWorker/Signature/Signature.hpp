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

	// 	SignatureRealization() = default;
	// 	SignatureRealization(std::variant<NativeRealization, MatherRealization>&& realiz, const unsigned char prior = 0, const SignatureType t = SignatureType::operation, OperatorPriority assitiat = OperatorPriority::none) :
	// 		realization{std::move(realiz)}, priority{prior}, type{t}, assitiation{assitiat}
	// 	{}

	// 	friend class SignatureNode;

	// private:

	// 	mutable bool taken_ = false;

	};

	using FunctionalContext = std::map<std::string, SignatureRealization, std::less<>>;
	using VariableContext = std::map<std::string, MathNodeP>;

	class Signature
	{
	public:
		Signature() = default;
		
		void addIfFunction(const std::string& name, NativeRealization&& realization)
		{
			ifFunction_ = name;
			functionalContext_[name] = SignatureRealization{ std::move(realization), 0, SignatureType::function };
		}
		void addFunction(const std::string& name, std::variant<NativeRealization, MatherRealization>&& realization)
		{
			functionalContext_[name] = SignatureRealization{ std::move(realization), 0, SignatureType::function };
		}
		void addSpecialFunction(const std::string& name, std::variant<NativeRealization, MatherRealization>&& realization)
		{
			functionalContext_[name] = SignatureRealization{ std::move(realization), 0, SignatureType::specialFunction };
		}
		void addUnareRightOperator(const std::string& name, NativeRealization&& realization)
		{
			functionalContext_[name] = SignatureRealization{ realization, 1, SignatureType::unareRight };
		}
		void addUnareLeftOperator(const std::string& name, NativeRealization&& realization)
		{
			functionalContext_[name] = SignatureRealization{ realization, 2, SignatureType::unareLeft };
		}
		void addOperator(const std::string& name, NativeRealization&& realization, const unsigned char priority, const OperatorPriority direction = OperatorPriority::leftToRight)
		{ 
			functionalContext_[name] = SignatureRealization{ realization, static_cast<unsigned char>(priority + 3), SignatureType::operation, direction };
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

		bool isTerm(const std::string_view str) const
		{
			return functionalContext_.find(str) != functionalContext_.end();
		}
		bool isIfFunction(const std::string_view str) const
		{
			return str == ifFunction_;
		}
		bool isSignatureType(const std::string_view str, const SignatureType type) const
		{
			const auto& found = functionalContext_.find(str);
			return found == functionalContext_.end() ? false : (found->second.type == type);
		}

		const SignatureRealization* operator[](const std::string_view str) const
		{
			return at(str);
		}
		SignatureRealization* operator[](const std::string_view str)
		{
			return at(str);
		}

		const SignatureRealization* at(const std::string_view str) const
		{
			auto found = functionalContext_.find(str);
			return (found == functionalContext_.end()) ? (nullptr) : (&found->second);
		}
		SignatureRealization* at(const std::string_view str)
		{
			auto found = functionalContext_.find(str);
			return (found == functionalContext_.end()) ? (nullptr) : (&found->second);
		}

		void addConstant(const std::string& name, MathNodeP v)
		{
			constantContext_[name] = std::move(v);
		}

		const VariableContext& getVariableContext() const
		{
			return constantContext_; 
		}

	private:

		FunctionalContext functionalContext_;
		VariableContext constantContext_;

		std::string defaultOperator_;
		std::string ifFunction_;

	};


}