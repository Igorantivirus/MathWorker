#pragma once

#include <complex>
#include <string>
#include <memory>
#include <map>
#include <functional>
#include <variant>
#include <vector>

namespace mathWorker
{

	using RealType = long double;
	using ComplexType = std::complex<RealType>;

	class MathNode;

	enum class SignatureType : unsigned char
	{
		operation = 0,
		function,
		unare,
		specialFunction
	};

	using MathNodeP = std::unique_ptr<MathNode>;
	using VariableContext = std::map<std::string, MathNodeP>;

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

	using MathVector = std::vector<MathNodeP>;
	using MathRowVector = std::vector<MathNode*>;


}