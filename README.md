<!--MathWorker-->
<h1 align="center">MathWorker</h1>

[![Русская версия](https://img.shields.io/badge/Русская%20версия-blue)](README_RU.md)

<h2>Description</h2>

A C++ library for processing mathematical expressions with a fully customizable signature.
It's capable of parsing mathematical expressions from a std::string.
You can add new constants, functions, and operators to the signature at runtime.

<h2>Dependencies</h2>

C++20.

<h2>Key Features</h2>

* ⚡ **Single-header file**
* 🚀 **Lightweight and fast**
* 🔗 **No external dependencies**
* 🛠️ **Fully customizable**
* ✅ **`<regex>`-independent**

<h2>Installation</h2>

1. **Clone the repository**
   ```sh
   git clone https://github.com/Igorantivirus/MathWorker
   ```
2. **Add the path to the include folder in your project settings**
   </br>
   Provide your project with the path to the include folder from the cloned repository. For example, in VS22, you can do this as follows:
   
   - **Visual Studio 2022**
     </br>
     Go to `Project Name`->`Properties`->`C/C++`->`General`
     </br>
     In the `Additional Include Directories`
     field, paste the path to the "include" folder.
3. **Include the header file in your code**
   ```cpp
   #include "MathWorker/MathWorker.hpp"
   ```

<h2>Documentation</h2>

Work in progress.

<h2>Examples</h2>

1. **Example 1**: Console Calculator

``` cpp
#include <iostream>

#include <MathWorker/MathWorker.hpp>

using namespace mathWorker;

int main()
{
	// Basic algebraic signature and constants
	Signature signature = generator::mathSignature();
	VariableContext constants = generator::baseConstants();

	MathParser parser(signature, std::make_unique<BaseTokenizer>(signature));
	FunctionConnector connector;

	std::string s;
	while (true)
	{
		try
		{
			std::getline(std::cin, s);
			std::cout << parser.parse(s)->replace(constants)->calculate(signature)->toString() << '\n';
		}
		catch (const ParseException& e)
		{
			std::cout << "Error: " << e.what() << ", type of error: " << static_cast<int>(e.type()) << '\n';
		}
		catch (...)
		{
			std::cout << "Unknown error" << '\n';
		}
	}
	return 0;
}
```
2. **Example 2**: Custom Signature

```cpp
#include <iostream>

#include <MathWorker/MathWorker.hpp>

using namespace mathWorker;

int main()
{
	Signature signature;
	VariableContext constants;
	MathParser parser(signature, std::make_unique<BaseTokenizer>(signature));
	
	// Custom operator
	signature.addOperator("@", [](const std::vector<MathNodeP>& params)->MathNodeP
	{
		ComplexType left = params[0]->getNumberForced();
		ComplexType right = params[1]->getNumberForced();
		ComplexType res = left + right + left * right;
		return std::make_unique<ValueNode>(res);
	}, 0);
	
	FunctionConnector connector;

	// Adding a function (one of 3 ways)
	connector.addFunction(signature, "f(x)=2@x@17");

	std::string s = "f(1)@2";
	std::cout << parser.parse(s)->replace(constants)->calculate(signature)->toString() << '\n';

	return 0;
}
```

**Output**: `323`


<h2>Road Map</h2>

1. Memory management overhaul
2. Add template support
3. Rework function addition (to prevent errors)
4. Output the parsed expression in LaTeX format
5. Release

<h2>License</h2>

**The MIT License**
