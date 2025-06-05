#include <iostream>

#include "ValueNode.hpp"
#include "SymbolNode.hpp"
#include "SignatureNode.hpp"

#include "Parser.hpp"

using namespace mathWorker;

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
SignatureContext generateBaseSignature()
{
	SignatureContext context;

	context["+"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = params[0]->getNumberForced() + params[1]->getNumberForced();
			return std::make_unique<ValueNode>(res);
		},0, SignatureType::operation
	};
	context["-"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = params[0]->getNumberForced() - params[1]->getNumberForced();
			return std::make_unique<ValueNode>(res);
		},0, SignatureType::operation
	};
	context["*"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = params[0]->getNumberForced() * params[1]->getNumberForced();
			return std::make_unique<ValueNode>(res);
		},1, SignatureType::operation
	};
	context["/"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = params[0]->getNumberForced() / params[1]->getNumberForced();
			return std::make_unique<ValueNode>(res);
		},1, SignatureType::operation
	};
	context["^"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = std::pow(params[0]->getNumberForced(), params[1]->getNumberForced());
			return std::make_unique<ValueNode>(res);
		},2, SignatureType::operation
	};

	context["sin"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = std::sin(params[0]->getNumberForced());
			return std::make_unique<ValueNode>(res);
		},3, SignatureType::function
	};
	context["cos"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = std::cos(params[0]->getNumberForced());
			return std::make_unique<ValueNode>(res);
		},3, SignatureType::function
	};
	context["tg"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = std::tan(params[0]->getNumberForced());
			return std::make_unique<ValueNode>(res);
		},3, SignatureType::function
	};
	context["ctg"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = ComplexType(1) / std::tan(params[0]->getNumberForced());
			return std::make_unique<ValueNode>(res);
		},3, SignatureType::function
	};

	context["arcsin"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = std::asin(params[0]->getNumberForced());
			return std::make_unique<ValueNode>(res);
		},3, SignatureType::function
	};
	context["arccos"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = std::acos(params[0]->getNumberForced());
			return std::make_unique<ValueNode>(res);
		},3, SignatureType::function
	};
	context["arctg"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = std::atan(params[0]->getNumberForced());
			return std::make_unique<ValueNode>(res);
		},3, SignatureType::function
	};
	context["arcctg"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = ComplexType(std::acos(-1) / 2) - std::atan(params[0]->getNumberForced());
			return std::make_unique<ValueNode>(res);
		},3, SignatureType::function
	};

	context["ln"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = std::log(params[0]->getNumberForced());
			return std::make_unique<ValueNode>(res);
		},3, SignatureType::function
	};
	context["exp"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = std::exp(params[0]->getNumberForced());
			return std::make_unique<ValueNode>(res);
		},3, SignatureType::function
	};
	context["log"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = std::log(params[1]->getNumberForced()) / std::log(params[0]->getNumberForced());
			return std::make_unique<ValueNode>(res);
		},3, SignatureType::specialFunction
	};
	context["root"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = pow(params[1]->getNumberForced(), ComplexType(1) / params[0]->getNumberForced());
			return std::make_unique<ValueNode>(res);
		},3, SignatureType::specialFunction
	};
	context["sqrt"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = std::sqrt(params[0]->getNumberForced());
			return std::make_unique<ValueNode>(res);
		},3, SignatureType::function
	};

	context["d"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = params[0]->getNumberForced() * ComplexType(180.l / std::acos(-1));
			return std::make_unique<ValueNode>(res);
		},4, SignatureType::unare
	};
	context["r"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = params[0]->getNumberForced() * ComplexType(std::acos(-1) / 180);
			return std::make_unique<ValueNode>(res);
		},4, SignatureType::unare
	};

	return context;
}

SignatureContext makeBoolContext()
{
	SignatureContext context;

	context["!"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = not(params[0]->getNumberForced().real());
			return std::make_unique<ValueNode>(res);
		},3, SignatureType::unare
	};
	context["&"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = params[0]->getNumberForced().real() && params[1]->getNumberForced().real();
			return std::make_unique<ValueNode>(res);
		},2, SignatureType::operation
	};
	context["|"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = params[0]->getNumberForced().real() || params[1]->getNumberForced().real();
			return std::make_unique<ValueNode>(res);
		},1, SignatureType::operation
	};
	context["->"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = not(params[0]->getNumberForced().real()) || params[1]->getNumberForced().real();
			return std::make_unique<ValueNode>(res);
		},1, SignatureType::operation
	};
	context["<->"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = params[0]->getNumberForced() == params[1]->getNumberForced();
			return std::make_unique<ValueNode>(res);
		},0, SignatureType::operation
	};
	return context;
}

MathNodeP makeMyFoo()
{
	SymbolNode x("x");
	SymbolNode y("y");
	ValueNode t(2);

	SignatureNode s1("^", { &x, &t });
	SignatureNode s2("^", { &y, &t });

	SignatureNode sum("+", MathRowVector{ &s1, &s2 });

	MathNodeP result(new SignatureNode{ "sqrt", MathRowVector{&sum} });

	return result;
}

void addFunction(SignatureContext& context, const std::string& name, const std::vector<std::string>& paramNames, const std::string realization, const unsigned char priority = 3, const SignatureType type = SignatureType::function)
{
	MathParser parser;
	parser.setContext(&context);

	MathNodeP node = parser.parse(realization);
	//std::cout << node->toString() << '\n';

	MatherRealization pair;
	pair.first = std::move(node);
	pair.second = paramNames;


	context[name].realization = std::move(pair);
	context[name].priority = priority;
	context[name].type = type;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v)
{
	if (v.empty())
		return out << "{}";
	out << '{' <<'\"' << v[0] << '\"';
	for (size_t i = 1; i < v.size(); ++i)
		out << ", \"" << v[i] << '\"';
	return out << '}';
}

std::string input(std::string s = {})
{
	std::cout << s;
	std::getline(std::cin, s);
	return s;
}

int main()
{
	SignatureContext context = generateBaseSignature();
	VariableContext constants = generateBaseConstants();

	MathParser parse;
	parse.setContext(&context);

	/*std::string s;

	while (true)
	{
		s = input("Enter: ");
		MathNodeP ptr = parse.parse(s);
		std::cout << ptr->toString() << '\n';
		std::cout << ptr->replace(constants)->calculate(context)->toString() << '\n';
	}*/


	std::string s = "log2(8)^2";
	//std::string s = "e^(2*i)-(cos(2)+i*sin(2))+e+exp(1)";

	MathNodeP ptr = parse.parse(s);

	std::cout << ptr->toString() << '\n';
	std::cout << ptr->replace(constants)->calculate(context)->toString() << '\n';

	




	//SignatureContext context = makeBaseContext();


	//addFunction(context, "size", { "x", "y", "z" }, "sqrt(x^2+y^2+z^2)");
	//addFunction(context, "squareRoot", { "a", "b", "c"}, "0-(b+sqrt(b^2-4*c*c))/(2*a)");





	//MathParser parse;
	//parse.setContext(&context);


	////std::string s = "squareRoot(1,2,1)";
	//std::string s = "2@3";


	/*MathNodeP ptr = parse.parse(s);

	std::cout << ptr->toString() << '\n';
	std::cout << ptr->calculate(context)->toString() << '\n';*/







	//std::string s = "1 + 2";
	//std::string s = "a-9.9999999+sin(3*pi-2*e/9)-(2+2)*2.98+2i-17sin(cos(5.5))+pi-e";
	//std::string s = "log2(7)";
		


	/*auto vec3 = parse.tokenize(s);

	std::cout << vec3 << '\n';*/

	/*auto vec = parse.tokenizer("a+sin(3*pi-2*e/9)");

	for (const auto& i : vec)
		std::cout << i << ' ';*/

	/*auto vec = parse.tokenizer("3*pi-2*e/9");

	for (const auto& i : vec)
		std::cout << i << ' ';*/

	

	/*SignatureContext funCont = makeBaseContext();
	MathNodeP realization = makeMyFoo();

	MatherRealization pair;
	pair.first = realization->clone();
	pair.second = std::vector<std::string>{ "x", "y" };

	funCont["abs"].realization = std::move(pair);



	ValueNode v1(ComplexType(3));
	ValueNode v2(ComplexType(4));

	SignatureNode sign("abs", MathRowVector{&v1, &v2});

	std::cout << sign.toString() << '\n';
	std::cout << sign.calculate(funCont)->toString() << '\n';*/




	return 0;
}