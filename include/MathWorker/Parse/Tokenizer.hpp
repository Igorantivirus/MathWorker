#pragma once

#include <span>
#include <string_view>
#include <vector>

#include "../MathUtils/MathDefines.hpp"
#include "ParseUtils.hpp"
#include "ParseException.hpp"
#include "../Signature/Signature.hpp"

namespace mathWorker
{

	using Token = std::string_view;
	using TokenArray = std::vector<Token>;
	using TokenArrayP = std::span<Token>;

	class Tokenizer
	{
	public:
		explicit Tokenizer(const Signature& signature);
		virtual ~Tokenizer() = default;

		void setSettings(const Signature& signature);
		const Signature& getSettings() const;

		virtual TokenArray tokenize(Token str) const = 0;
		virtual TokenArray tokenizeByComma(Token str) const = 0;

	protected:

		std::reference_wrapper<const Signature> signature_;

	};

	class BaseTokenizer : public Tokenizer
	{
	public:
		explicit BaseTokenizer(const Signature& signature);

		TokenArray tokenize(Token str) const override;
		TokenArray tokenizeByComma(Token str) const override;

	private:

		using CheckMethod = bool(*)(char);

		bool meansDefaultOperator(Token a, Token b) const;

		size_t getEndOf(Token str, size_t i, CheckMethod check) const;
		size_t getEndOfBracket(Token str, size_t i) const;

		TokenArray parseToTokens(Token str) const;

		TokenArray addDefaultOperator(TokenArray tkns) const;

	};
}
