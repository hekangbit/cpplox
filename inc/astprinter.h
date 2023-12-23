#include "common.h"
#include "expr.h"

class AstPrinter : public Visitor {
public:
  AstPrinter(){};
  ~AstPrinter(){};
  void Walk(Expr &expr);
  virtual void Visit(NumberLiteralExpr &expr);
  virtual void Visit(StringLiteralExpr &expr);
  virtual void Visit(UnaryExpr &expr);
  virtual void Visit(BinaryExpr &expr);
  virtual void Visit(GroupingExpr &expr);
  string Get() { return str; };

private:
  string str;
};