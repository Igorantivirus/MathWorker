#pragma once

#include "Signature.hpp"
#include "Parser.hpp"

namespace mathWorker
{

	Signature generateMathSignature()
	{
		Signature signature;

		signature.addOperator("+", [](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = params[0]->getNumberForced() + params[1]->getNumberForced();
				return std::make_unique<ValueNode>(res);
			}, 2, OperatorPriority::leftToRight);
		signature.addOperator("-", [](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res;
				if (params.size() == 1)
					res = ComplexType(0) - params[0]->getNumberForced();
				else
					res = params[0]->getNumberForced() - params[1]->getNumberForced();
				return std::make_unique<ValueNode>(res);
			}, 2, OperatorPriority::leftToRight);
		signature.addOperator("*", [](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = params[0]->getNumberForced() * params[1]->getNumberForced();
				return std::make_unique<ValueNode>(res);
			}, 1, OperatorPriority::leftToRight);
		signature.addOperator("/", [](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = params[0]->getNumberForced() / params[1]->getNumberForced();
				return std::make_unique<ValueNode>(res);
			}, 1, OperatorPriority::leftToRight);
		signature.addOperator("^", [](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType a = params[0]->getNumberForced();
				ComplexType b = params[1]->getNumberForced();
				bool isR_a = std::abs(a.imag()) < 1e-16;
				bool isR_b = std::abs(b.imag()) < 1e-16;
				ComplexType res{};
				if (isR_a && isR_b)
					res = std::pow(a.real(), b.real());
				else
					res = std::pow(a, b);
				return std::make_unique<ValueNode>(res);
			}, 0, OperatorPriority::rightToLeft);

		//signature.addUnareRightOperator("d", [](const std::vector<MathNodeP>& params)->MathNodeP
		//	{
		//		ComplexType res = params[0]->getNumberForced() * ComplexType(180.l / std::acos(-1));
		//		return std::make_unique<ValueNode>(res);
		//	});
		//signature.addUnareRightOperator("r", [](const std::vector<MathNodeP>& params)->MathNodeP
		//	{
		//		ComplexType res = params[0]->getNumberForced() * ComplexType(std::acos(-1) / 180.l);
		//		return std::make_unique<ValueNode>(res);
		//	});

		signature.addFunction("sin",	[](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = std::sin(params[0]->getNumberForced());
				return std::make_unique<ValueNode>(res);
			});
		signature.addFunction("cos",	[](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = std::cos(params[0]->getNumberForced());
				return std::make_unique<ValueNode>(res);
			});
		signature.addFunction("tg",		[](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = std::tan(params[0]->getNumberForced());
				return std::make_unique<ValueNode>(res);
			});
		signature.addFunction("ctg",	[](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = ComplexType(1) / std::tan(params[0]->getNumberForced());
				return std::make_unique<ValueNode>(res);
			});
		signature.addFunction("arcsin",	[](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = std::asin(params[0]->getNumberForced());
				return std::make_unique<ValueNode>(res);
			});
		signature.addFunction("arccos",	[](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = std::acos(params[0]->getNumberForced());
				return std::make_unique<ValueNode>(res);
			});
		signature.addFunction("arctg",	[](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = std::atan(params[0]->getNumberForced());
				return std::make_unique<ValueNode>(res);
			});
		signature.addFunction("arcctg",	[](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = ComplexType(std::acos(-1) / 2) - std::atan(params[0]->getNumberForced());
				return std::make_unique<ValueNode>(res);
			});
		signature.addFunction("ln",		[](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = std::log(params[0]->getNumberForced());
				return std::make_unique<ValueNode>(res);
			});
		signature.addFunction("exp",	[](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = std::exp(params[0]->getNumberForced());
				return std::make_unique<ValueNode>(res);
			});
		signature.addFunction("sqrt",	[](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = std::sqrt(params[0]->getNumberForced());
				return std::make_unique<ValueNode>(res);
			});
		signature.addFunction("pow",	[](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType a = params[0]->getNumberForced();
				ComplexType b = params[1]->getNumberForced();
				bool isR_a = std::abs(a.imag()) < 1e-16;
				bool isR_b = std::abs(b.imag()) < 1e-16;
				ComplexType res{};
				if (isR_a && isR_b)
					res = std::pow(a.real(), b.real());
				else
					res = std::pow(a, b);
				return std::make_unique<ValueNode>(res);
			});
		signature.addFunction("conj",	[](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = std::conj(params[0]->getNumberForced());
				return std::make_unique<ValueNode>(res);
			});
		signature.addFunction("imag",	[](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = params[0]->getNumberForced().imag();
				return std::make_unique<ValueNode>(res);
			});
		signature.addFunction("real",	[](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = params[0]->getNumberForced().real();
				return std::make_unique<ValueNode>(res);
			});
		signature.addFunction("abs", [](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = std::abs(params[0]->getNumberForced());
				return std::make_unique<ValueNode>(res);
			});

		signature.addSpecialFunction("log", [](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = std::log(params[1]->getNumberForced()) / std::log(params[0]->getNumberForced());
				return std::make_unique<ValueNode>(res);
			});
		signature.addSpecialFunction("root", [](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType a = params[1]->getNumberForced();
				ComplexType r = params[0]->getNumberForced();
				
				bool isR_a = std::abs(a.imag()) < 1e-16;
				bool isR_r = std::abs(r.imag()) < 1e-16;
				ComplexType res{};
				if (isR_a && isR_r && std::fmod(r.real(), 2) == 1 && a.real() < 0)
					res = -std::pow(-a.real(), 1 / r.real());
				else
					res = std::pow(a, ComplexType(1) / r);
				return std::make_unique<ValueNode>(res);


				/*if(isR(root) && std::fmod(root.real(), 2) == 1 && isR(a) && a.real() < 0)
				return - std::pow(-a.real(), 1 / root.real());
			return std::pow(a, Complex(1) / root);*/
				/*ComplexType res = std::pow(params[1]->getNumberForced(), ComplexType(1) / params[0]->getNumberForced());
				return std::make_unique<ValueNode>(res);*/
			});

		signature.setDefaultOperator("*");

		return signature;
	}

	Signature generateBoolSignature()
	{
		Signature signature;

		signature.addUnareLeftOperator("!", [](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = not(params[0]->getNumberForced().real());
				return std::make_unique<ValueNode>(res);
			});

		signature.addOperator("&", [](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = params[0]->getNumberForced().real() && params[1]->getNumberForced().real();
				return std::make_unique<ValueNode>(res);
			}, 1);
		signature.addOperator("|", [](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = params[0]->getNumberForced().real() || params[1]->getNumberForced().real();
				return std::make_unique<ValueNode>(res);
			}, 2);
		signature.addOperator("->", [](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = not(params[0]->getNumberForced().real()) || params[1]->getNumberForced().real();
				return std::make_unique<ValueNode>(res);
			}, 2);
		signature.addOperator("+", [](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = params[0]->getNumberForced() != params[1]->getNumberForced();
				return std::make_unique<ValueNode>(res);
			}, 3);
		signature.addOperator("=", [](const std::vector<MathNodeP>& params)->MathNodeP
			{
				ComplexType res = params[0]->getNumberForced() == params[1]->getNumberForced();
				return std::make_unique<ValueNode>(res);
			}, 3);

		return signature;
	}

	VariableContext generateBaseConstants()
	{
		VariableContext context;
		context["pi"]	= ValueNode(RealType(std::acos(-1))).clone();
		context["e"]	= ValueNode(std::exp(1)).clone();
		context["phi"]	= ValueNode(RealType(0.5l + std::sqrt(5) / 2.l)).clone();
		context["i"]	= ValueNode(ComplexType(0, 1)).clone();
		context["inf"]	= ValueNode(ComplexType(RealType(1.) / RealType(sin(0)), 0)).clone();
		return context;
	}



}