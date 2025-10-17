#pragma once

#include <string_view>
#include <variant>

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
	enum class ArgEvalPolicy : bool
	{
		eagar = false,
		lazy = true
	};

	struct SignatureRealization
	{
		std::variant<NativeRealization, MatherRealization> realization;
		unsigned char priority = 0;
		SignatureType type = SignatureType::operation;
		OperatorPriority assitiation = OperatorPriority::none;
		ArgEvalPolicy policy = ArgEvalPolicy::eagar;
	};

	using FunctionalContext = std::map<std::string, SignatureRealization, std::less<>>;
	using VariableContext = std::map<std::string, MathNodeP>;

	class Signature
	{
	public:
		Signature();

		void addFunction(const std::string& name, std::variant<NativeRealization, MatherRealization>&& realization, ArgEvalPolicy policy = ArgEvalPolicy::eagar);
		void addSpecialFunction(const std::string& name, std::variant<NativeRealization, MatherRealization>&& realization);
		void addUnareRightOperator(const std::string& name, NativeRealization&& realization);
		void addUnareLeftOperator(const std::string& name, NativeRealization&& realization);
		void addOperator(const std::string& name, NativeRealization&& realization, unsigned char priority, OperatorPriority direction = OperatorPriority::leftToRight, ArgEvalPolicy policy = ArgEvalPolicy::eagar);

		void setDefaultOperator(const std::string& defautlOperator);
		const std::string& getDefaultOperator() const;

		bool isTerm(std::string_view str) const;
		bool isSignatureType(std::string_view str, SignatureType type) const;

		const SignatureRealization* operator[](std::string_view str) const;
		SignatureRealization* operator[](std::string_view str);

		const SignatureRealization* at(std::string_view str) const;
		SignatureRealization* at(std::string_view str);

		void addConstant(const std::string& name, MathNodeP v);

		const VariableContext& getVariableContext() const;

	private:

		FunctionalContext functionalContext_;
		VariableContext constantContext_;

		std::string defaultOperator_;

	};


}
