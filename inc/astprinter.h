#include "common.h"
#include "expr.h"
#include <vector>

class AstPrinter : public Visitor {
public:
  AstPrinter() {}
  ~AstPrinter() {}
  void Walk(Expr &expr);
  virtual void Visit(NumberLiteralExpr &expr);
  virtual void Visit(StringLiteralExpr &expr);
  virtual void Visit(UnaryExpr &expr);
  virtual void Visit(BinaryExpr &expr);
  virtual void Visit(GroupingExpr &expr);
  virtual void Visit(NullLiteralExpr &expr);
  virtual void Visit(BoolLiteralExpr &expr);
  string Get() { return str; };
  void Parenthesize(string &name, vector<Expr *> &exprs);

private:
  string str;
};