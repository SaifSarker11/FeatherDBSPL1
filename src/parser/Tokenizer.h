#ifndef SPL_TOKENIZER_H
#define SPL_TOKENIZER_H

#include <string>
#include <vector>

namespace spl {

class Tokenizer {
public:
	Tokenizer() = default;
	virtual ~Tokenizer() = default;

	std::vector<std::string> tokenize(const std::string &input) const;
};

} // namespace spl

#endif // SPL_TOKENIZER_H