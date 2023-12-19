#include "common.h"
#include "expr.h"

template<typename T>
class Visitor {
public:
  virtual T visit(NumberLiteralExpr expr);
  virtual T visit(StringLiteralExpr expr);
  virtual T visit(UnaryExpr expr);
  virtual T visit(BinaryExpr expr);
};