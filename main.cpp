#include <iostream>

#include "ValueNode.hpp"
#include "SymbolNode.hpp"
#include "SignatureNode.hpp"

#include "Parser.hpp"

using namespace mathWorker;

SignatureContext makeBaseContext()
{
	SignatureContext context;

	context["+"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = params[0]->getNumberForced() + params[1]->getNumberForced();
			return std::make_unique<ValueNode>(res);
		},0
	};
	context["-"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = params[0]->getNumberForced() - params[1]->getNumberForced();
			return std::make_unique<ValueNode>(res);
		},0
	};
	context["*"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = params[0]->getNumberForced() * params[1]->getNumberForced();
			return std::make_unique<ValueNode>(res);
		},1
	};
	context["/"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = params[0]->getNumberForced() / params[1]->getNumberForced();
			return std::make_unique<ValueNode>(res);
		},1
	};
	context["^"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = std::pow(params[0]->getNumberForced(), params[1]->getNumberForced());
			return std::make_unique<ValueNode>(res);
		},2
	};
	context["sqrt"] =
	{
		[](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = std::sqrt(params[0]->getNumberForced());
			return std::make_unique<ValueNode>(res);
		},3
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

int main()
{
	MathParser parse;

	/*auto vec = parse.tokenizer("a+sin(3*pi-2*e/9)");

	for (const auto& i : vec)
		std::cout << i << ' ';*/

	/*auto vec = parse.tokenizer("3*pi-2*e/9");

	for (const auto& i : vec)
		std::cout << i << ' ';*/

	

	SignatureContext funCont = makeBaseContext();
	MathNodeP realization = makeMyFoo();

	MatherRealization pair;
	pair.first = realization->clone();
	pair.second = std::vector<std::string>{ "x", "y" };

	funCont["abs"].realization = std::move(pair);



	ValueNode v1(ComplexType(3));
	ValueNode v2(ComplexType(4));

	SignatureNode sign("abs", MathRowVector{&v1, &v2});

	std::cout << sign.toString() << '\n';
	std::cout << sign.calculate(funCont)->toString() << '\n';




	return 0;
}