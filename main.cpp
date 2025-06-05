#include <iostream>

#include "ValueNode.hpp"
#include "SymbolNode.hpp"
#include "SignatureNode.hpp"

using namespace mathWorker;

FunctionContext makeBaseContext()
{
	FunctionContext context;

	context["+"] = [](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = params[0]->getNumberForced() + params[1]->getNumberForced();
			return std::make_unique<ValueNode>(res);
		};
	context["-"] = [](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = params[0]->getNumberForced() - params[1]->getNumberForced();
			return std::make_unique<ValueNode>(res);
		};
	context["*"] = [](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = params[0]->getNumberForced() * params[1]->getNumberForced();
			return std::make_unique<ValueNode>(res);
		};
	context["/"] = [](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = params[0]->getNumberForced() / params[1]->getNumberForced();
			return std::make_unique<ValueNode>(res);
		};
	context["^"] = [](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType res = std::pow(params[0]->getNumberForced(), params[1]->getNumberForced());
			return std::make_unique<ValueNode>(res);
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

	MathNodeP result(new SignatureNode{ "+", MathRowVector{&s1, &s2} });

	return result;
}

int main()
{
	FunctionContext funCont = makeBaseContext();
	MathNodeP realization = makeMyFoo();

	MatherRealization pair;
	pair.first = realization->clone();
	pair.second = std::vector<std::string>{ "x", "y" };

	funCont["foo"] = std::move(pair);



	ValueNode v1(ComplexType(3));
	ValueNode v2(ComplexType(4));

	SignatureNode sign("foo", MathRowVector{&v1, &v2});

	std::cout << sign.toString() << '\n';
	std::cout << sign.calculate(funCont)->toString() << '\n';




	return 0;
}