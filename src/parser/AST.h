#ifndef SPL_AST_H
#define SPL_AST_H

#include <memory>

namespace spl {

struct AstNode {
	virtual ~AstNode() = default;
};

using Ast = std::shared_ptr<AstNode>;

} // namespace spl

#endif // SPL_AST_H