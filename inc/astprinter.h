#include "common.h"
#include "visitor.h"


class AstPrinter : public Visitor<string> {
public:
  void print(Expr expr);
  virtual string visit(NumberLiteralExpr expr);
  virtual string visit(StringLiteralExpr expr);
  virtual string visit(UnaryExpr expr);
  virtual string visit(BinaryExpr expr);
};