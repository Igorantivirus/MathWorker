<!--MathWorker-->
<h1 align="center">MathWorker</h1>

[![English version](https://img.shields.io/badge/English%version-blue)](README.md)

<h2>Описание</h2>

С++ библиотека для обработки математических выражений с полностью настраиваемой сигнатурой. 
Способка парсить математические выражения из std строки.
Можно добавлять в процессе работы новые константы, функции и операторы для сигнатуры.

<h2>Зависимости</h2>

C++20.

<h2>Ключевые особенности</h2>

* ⚡ **Один заголовочный файл**
* 🚀 **Легковесная и быстрая**
* 🔗 **Без дополнительных библиотек**
* 🛠️ **Полностью настраиваемая**
* ✅ **Независима от `<regex`>**

<h2>Установка</h2>

1. **Клонирование репозитория**
   ```sh
   git clone https://github.com/Igorantivirus/MathWorker
   ```
2. **Добавьте путь к папке include в параметрах проекта**
   </br>
   Дайте проетку путь к папке include из распакованного репозитория, например, для VS22 это делается следующим образом:
   
   - **Visual Studio 2022**
     </br>
     Перейдите в `Имя проекта`->`Свойства`->`C/C++`->`Общее`
     </br>
     В поле `Дополнительные катаолги включаемых файлов`
     вставьте путь до папки "include".
3. **Подключите заголовочный файл в коде**
   ```cpp
   #include "MathWorker/MathWorker.hpp"
   ```

<h2>Документация</h2>

В разработке

<h2>Примеры</h2>

1. **Пример 1**: Консольный калькулятор

``` cpp
#include <iostream>

#include <MathWorker/MathWorker.hpp>

using namespace mathWorker;

int main()
{
	//Основная сигнатура и константы алгебры
	Signature signature = generator::mathSignature();
	VariableContext constants = generator::baseConstants();
	
	BaseTokenizer tokenizer(&signature);
	MathParser parser(&signature, &tokenizer);
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
			std::cout << "Unlnown error" << '\n';
		}
	}
	return 0;
}
```
2. **Пример 2** Своя сигнатура

```cpp
#include <iostream>

#include <MathWorker/MathWorker.hpp>

using namespace mathWorker;

int main()
{
	Signature signature;
	VariableContext constants;
	BaseTokenizer tokenizer(&signature);
	MathParser parser(&signature, &tokenizer);
	
	//Кастомный оператор
	signature.addOperator("@", [](const std::vector<MathNodeP>& params)->MathNodeP
	{
		ComplexType left = params[0]->getNumberForced();
		ComplexType right = params[1]->getNumberForced();
		ComplexType res = left + right + left * right;
		return std::make_unique<ValueNode>(res);
	}, 0);
	
	FunctionConnector connector;

	//Добавление функции (один из 3-х способов)
	connector.addFunction(signature, "f(x)=2@x@17");

	std::string s = "f(1)@2";
	std::cout << parser.parse(s)->replace(constants)->calculate(signature)->toString() << '\n';

	return 0;
}
```

**Вывод**: `323`


<h2>Дорожная карта</h2>

1. Переработка работы с памятью
2. Добавление шаблонности
3. Перерабокта добавления функций (для предотвращения ошибок)
4. Вывод выражения после парсинга в LaTex
5. Релиз

<h2>Лицензия</h2>

**The MIT License**