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

	using MathNodeP = std::unique_ptr<MathNode>;
	using VariableContext = std::map<std::string, MathNodeP>;

	using NativeRealization = std::function<MathNodeP(const std::vector<MathNodeP>&)>;
	using MatherRealization = std::pair<MathNodeP, std::vector<std::string>>;

	struct SignatureRealization
	{
		std::variant<NativeRealization, MatherRealization> realization;
		unsigned char priority = 0;
	};

	using SignatureContext = std::map<std::string, SignatureRealization>;

	using MathVector = std::vector<MathNodeP>;
	using MathRowVector = std::vector<MathNode*>;


}