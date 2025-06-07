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
		unareLeft,
		unareRight,
		function,
		specialFunction
	};

	using MathNodeP = std::unique_ptr<MathNode>;
	using VariableContext = std::map<std::string, MathNodeP>;

	

	using MathVector = std::vector<MathNodeP>;
	using MathRowVector = std::vector<MathNode*>;


}