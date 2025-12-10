#include "MathWorker/Signature/SignatureGenerator.hpp"

#include <cmath>
#include <stdexcept>

#include "MathWorker/MathClasses/SignatureNode.hpp"
#include "MathWorker/MathClasses/ValueNode.hpp"
#include "MathWorker/MathUtils/MathDefines.hpp"

namespace mathWorker
{

	namespace generator
	{

		Signature mathSignature()
		{
			Signature signature;

#define CheckSize(params, s) if ((params).size() != (s)) throw std::logic_error("The number of signature arguments does not match the expected number.");

			signature.addOperator("?", [&signature](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 2);
					ComplexType ifV = params[0]->calculate(signature)->getNumberForced();
					const MathNodeP& first = params[1];
					const SignatureNode* node = dynamic_cast<const SignatureNode*>(first.get());
					if (!node)
						return std::make_unique<ValueNode>();

					const MathVector& ifParams = node->getParams();
					if (std::abs(ifV.imag()) <= 1e-16f && std::abs(ifV.real()) <= 1e-16f)
						return ifParams[1]->calculate(signature);
					return ifParams[0]->calculate(signature);
				}, 4, OperatorPriority::rightToLeft, ArgEvalPolicy::lazy);
			signature.addOperator(":", [](const std::vector<MathNodeP>&) -> MathNodeP
				{
					throw std::logic_error("The operator ':' shouldn't exist without a corresponding '?' the operator.");
					return std::make_unique<ValueNode>();
				}, 4, OperatorPriority::rightToLeft);
			signature.addOperator("<", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 2);
					ComplexType a = params[0]->getNumberForced();
					ComplexType b = params[1]->getNumberForced();
					if (std::abs(a.imag()) <= 1.e-16l && std::abs(b.imag()) <= 1.e-16l)
						return std::make_unique<ValueNode>(a.real() < b.real());
					return std::make_unique<ValueNode>(ComplexType(std::nanl(""), std::nanl("")));
				}, 3, OperatorPriority::rightToLeft);
			signature.addOperator("<=", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 2);
					ComplexType a = params[0]->getNumberForced();
					ComplexType b = params[1]->getNumberForced();
					if (std::abs(a.imag()) <= 1.e-16l && std::abs(b.imag()) <= 1.e-16l)
						return std::make_unique<ValueNode>(a.real() <= b.real());
					return std::make_unique<ValueNode>(ComplexType(std::nanl(""), std::nanl("")));
				}, 3, OperatorPriority::rightToLeft);
			signature.addOperator(">", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 2);
					ComplexType a = params[0]->getNumberForced();
					ComplexType b = params[1]->getNumberForced();
					if (std::abs(a.imag()) <= 1.e-16l && std::abs(b.imag()) <= 1.e-16l)
						return std::make_unique<ValueNode>(a.real() > b.real());
					return std::make_unique<ValueNode>(ComplexType(std::nanl(""), std::nanl("")));
				}, 3, OperatorPriority::rightToLeft);
			signature.addOperator(">=", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 2);
					ComplexType a = params[0]->getNumberForced();
					ComplexType b = params[1]->getNumberForced();
					if (std::abs(a.imag()) <= 1.e-16l && std::abs(b.imag()) <= 1.e-16l)
						return std::make_unique<ValueNode>(a.real() >= b.real());
					return std::make_unique<ValueNode>(ComplexType(std::nanl(""), std::nanl("")));
				}, 3, OperatorPriority::rightToLeft);
			signature.addOperator("==", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 2);
					ComplexType a = params[0]->getNumberForced();
					ComplexType b = params[1]->getNumberForced();
					return std::make_unique<ValueNode>(ComplexType(a == b, 0));
				}, 3, OperatorPriority::rightToLeft);
			signature.addOperator("!=", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 2);
					ComplexType a = params[0]->getNumberForced();
					ComplexType b = params[1]->getNumberForced();
					return std::make_unique<ValueNode>(ComplexType(a != b, 0));
				}, 3, OperatorPriority::rightToLeft);
			signature.addOperator("+", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 2);
					ComplexType res = params[0]->getNumberForced() + params[1]->getNumberForced();
					return std::make_unique<ValueNode>(res);
				}, 2, OperatorPriority::leftToRight);
			signature.addOperator("-", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					if (params.size() > 2)
						throw std::logic_error("The number of signature arguments does not match the expected number.");
					ComplexType res;
					if (params.size() == 1)
						res = ComplexType(0) - params[0]->getNumberForced();
					else
						res = params[0]->getNumberForced() - params[1]->getNumberForced();
					return std::make_unique<ValueNode>(res);
				}, 2, OperatorPriority::leftToRight);
			signature.addOperator("*", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 2);
					ComplexType res = params[0]->getNumberForced() * params[1]->getNumberForced();
					return std::make_unique<ValueNode>(res);
				}, 1, OperatorPriority::leftToRight);
			signature.addOperator("/", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 2);
					ComplexType res = params[0]->getNumberForced() / params[1]->getNumberForced();
					return std::make_unique<ValueNode>(res);
				}, 1, OperatorPriority::leftToRight);
			signature.addOperator("^", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 2);
					ComplexType res = std::pow(params[0]->getNumberForced(), params[1]->getNumberForced());
					return std::make_unique<ValueNode>(res);
				}, 0, OperatorPriority::rightToLeft);
			signature.addOperator("%", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 2);
					return std::make_unique<ValueNode>(std::fmod(params[0]->getNumberForced().real(), params[1]->getNumberForced().real()));
				}, 1);
			signature.addOperator("xor", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 2);
					return std::make_unique<ValueNode>(params[0]->getNumberForced() == params[1]->getNumberForced());
				}, 1);
			signature.addFunction("sin", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 1);
					ComplexType res = std::sin(params[0]->getNumberForced());
					return std::make_unique<ValueNode>(res);
				});
			signature.addFunction("cos", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 1);
					ComplexType res = std::cos(params[0]->getNumberForced());
					return std::make_unique<ValueNode>(res);
				});
			signature.addFunction("tan", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 1);
					ComplexType res = std::tan(params[0]->getNumberForced());
					return std::make_unique<ValueNode>(res);
				});
			signature.addFunction("asin", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 1);
					ComplexType res = std::asin(params[0]->getNumberForced());
					return std::make_unique<ValueNode>(res);
				});
			signature.addFunction("acos", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 1);
					ComplexType res = std::acos(params[0]->getNumberForced());
					return std::make_unique<ValueNode>(res);
				});
			signature.addFunction("atan", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 1);
					ComplexType res = std::atan(params[0]->getNumberForced());
					return std::make_unique<ValueNode>(res);
				});
			signature.addFunction("atan2", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 2);
					ComplexType res = std::atan2(params[0]->getNumberForced().real(), params[1]->getNumberForced().real());
					return std::make_unique<ValueNode>(res);
				});
			signature.addFunction("sinh", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 1);
					ComplexType res = std::sinh(params[0]->getNumberForced());
					return std::make_unique<ValueNode>(res);
				});
			signature.addFunction("cosh", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 1);
					ComplexType res = std::cosh(params[0]->getNumberForced());
					return std::make_unique<ValueNode>(res);
				});
			signature.addFunction("tanh", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 1);
					ComplexType res = std::tanh(params[0]->getNumberForced());
					return std::make_unique<ValueNode>(res);
				});
			signature.addFunction("sqrt", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 1);
					ComplexType res = std::sqrt(params[0]->getNumberForced());
					return std::make_unique<ValueNode>(res);
				});
			signature.addFunction("pow", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 2);
					ComplexType a = params[0]->getNumberForced();
					ComplexType b = params[1]->getNumberForced();
					bool isR_a = std::abs(a.imag()) < 1e-16;
					bool isR_b = std::abs(b.imag()) < 1e-16;
					ComplexType res{};
					if (isR_a && isR_b && a.real() >= 0.l)
						res = std::pow(a.real(), b.real());
					else
						res = std::pow(a, b);
					return std::make_unique<ValueNode>(res);
				});
			signature.addFunction("exp", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 1);
					ComplexType a = params[0]->getNumberForced();
					ComplexType res = std::exp(a);
					return std::make_unique<ValueNode>(res);
				});
			signature.addFunction("ln", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 1);
					ComplexType a = params[0]->getNumberForced();
					ComplexType res = std::log(a);
					return std::make_unique<ValueNode>(res);
				});
			signature.addFunction("conj", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 1);
					ComplexType res = std::conj(params[0]->getNumberForced());
					return std::make_unique<ValueNode>(res);
				});
			signature.addFunction("imag", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 1);
					ComplexType res = params[0]->getNumberForced().imag();
					return std::make_unique<ValueNode>(res);
				});
			signature.addFunction("real", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 1);
					ComplexType res = params[0]->getNumberForced().real();
					return std::make_unique<ValueNode>(res);
				});
			signature.addFunction("abs", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 1);
					ComplexType res = std::abs(params[0]->getNumberForced());
					return std::make_unique<ValueNode>(res);
				});

			signature.addSpecialFunction("log", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 2);
					ComplexType res = std::log(params[1]->getNumberForced()) / std::log(params[0]->getNumberForced());
					return std::make_unique<ValueNode>(res);
				});
			signature.addSpecialFunction("root", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					CheckSize(params, 2);
					ComplexType r = params[0]->getNumberForced();
					ComplexType a = params[1]->getNumberForced();

					bool isR_a = std::abs(a.imag()) < 1e-16;
					bool isR_r = std::abs(r.imag()) < 1e-16;
					ComplexType res{};
					if (isR_a && isR_r && std::fmod(r.real(), 2) == 1 && a.real() < 0)
						res = -std::pow(-a.real(), 1 / r.real());
					else
						res = std::pow(a, ComplexType(1) / r);
					return std::make_unique<ValueNode>(res);
				});

			signature.setDefaultOperator("*");

			signature.addConstant("pi", std::make_unique<ValueNode>(ValueNode(RealType(std::acos(-1)))));
			signature.addConstant("e", std::make_unique<ValueNode>(ValueNode(std::exp(1))));
			signature.addConstant("phi", std::make_unique<ValueNode>(ValueNode(RealType(0.5l + std::sqrt(5) / 2.l))));
			signature.addConstant("i", std::make_unique<ValueNode>(ValueNode(ComplexType(0, 1))));
			signature.addConstant("inf", std::make_unique<ValueNode>(ValueNode(ComplexType(RealType(1.) / RealType(std::sin(0)), 0))));

#undef CheckSize

			return signature;
		}

		Signature boolSignature()
		{
			Signature signature;

			signature.addUnareLeftOperator("!", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					ComplexType res = not (params[0]->getNumberForced().real());
					return std::make_unique<ValueNode>(res);
				});

			signature.addOperator("&", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					ComplexType res = params[0]->getNumberForced().real() && params[1]->getNumberForced().real();
					return std::make_unique<ValueNode>(res);
				}, 0);
			signature.addOperator("|", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					ComplexType res = params[0]->getNumberForced().real() || params[1]->getNumberForced().real();
					return std::make_unique<ValueNode>(res);
				}, 1);
			signature.addOperator("->", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					ComplexType res = not (params[0]->getNumberForced().real()) || params[1]->getNumberForced().real();
					return std::make_unique<ValueNode>(res);
				}, 1);
			signature.addOperator("+", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					ComplexType res = params[0]->getNumberForced() != params[1]->getNumberForced();
					return std::make_unique<ValueNode>(res);
				}, 2);
			signature.addOperator("=", [](const std::vector<MathNodeP>& params) -> MathNodeP
				{
					ComplexType res = params[0]->getNumberForced() == params[1]->getNumberForced();
					return std::make_unique<ValueNode>(res);
				}, 2);

			return signature;
		}

		Signature mathVariableSumnature()
		{
			// TODO:
			return {};
		}

	}

}
