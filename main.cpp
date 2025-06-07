#include <iostream>

#include "ValueNode.hpp"
#include "SymbolNode.hpp"
#include "SignatureNode.hpp"

#include "SignatureGenerator.hpp"

#include "Parser.hpp"

using namespace mathWorker;

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
	try
	{
		Signature signature = generateMathSignature();
		VariableContext constants = generateBaseConstants();
		BaseTokenizer tokenizer(&signature);
		MathParser parser(&signature, &tokenizer);


		std::string s;

		/*while (true)
		{
			s = input("Enter: ");
			auto ar = tokenizer.tokenize(s);
			std::cout << ar << '\n';
		}*/


		//addFunction(signature, "sqrr", { "a", "b", "c" }, "-(b-sqrt(b^2-4a c))/(2a)");
		//addFunction(signature, "abs", { "z" }, "sqrt(real(z)^2+imag(z)^2)");
		//addFunction(signature, "foo", { "x","y" }, "1 / (x ^ 2 * y ^ 2)");
		//addFunction(signature, "f", { "x","y" }, "x+y+x y");


		while (true)
		{
			s = input("Enter: ");
			MathNodeP ptr = parser.parse(s);
			std::cout << ptr->toString() << '\n';
			std::cout << ptr->replace(constants)->calculate(signature)->toString() << '\n';
		}
	}
	catch (const ParseException& e)
	{
		std::cout << e.what() << ' ' << static_cast<int>(e.type()) << '\n';
	}

	return 0;
}