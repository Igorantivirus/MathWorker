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
	SymbolNode a("a");
	SymbolNode b("b");
	ValueNode two(ComplexType(2));

	std::vector<MathNodeP> params;

	params.push_back(a.clone());
	params.push_back(two.clone());
	SignatureNode s1("^", params);

	params.clear();

	params.push_back(b.clone());
	params.push_back(two.clone());
	SignatureNode s2("^", params);

	params.clear();

	params.push_back(s1.clone());
	params.push_back(s2.clone());

	SignatureNode res("+", params);
	return res.clone();
}

int main()
{
	FunctionContext funCont = makeBaseContext();
	MathNodeP realization = makeMyFoo();

	MatherRealization pair;
	pair.first = realization->clone();
	pair.second = std::vector<std::string>{ "a", "b" };

	funCont["foo"] = std::move(pair);



	ValueNode v1(ComplexType(3));
	ValueNode v2(ComplexType(4));

	std::vector<MathNodeP> params;
	params.push_back(v1.clone());
	params.push_back(v2.clone());

	SignatureNode sign("foo", params);

	std::cout << sign.toString() << '\n';
	std::cout << sign.calculate(funCont)->toString() << '\n';




	return 0;
}