#include "common.h"
#include "expr.h"
#include "stmt.h"
#include "visitor.h"
#include <vector>

class AstPrinter : public Visitor {
public:
  AstPrinter() {}
  ~AstPrinter() {}
  void Walk(Expr &expr);
  string Parenthesize(string &name, vector<Expr *> &exprs);
  virtual LoxValue Visit(NumberLiteralExpr &expr);
  virtual LoxValue Visit(StringLiteralExpr &expr);
  virtual LoxValue Visit(UnaryExpr &expr);
  virtual LoxValue Visit(BinaryExpr &expr);
  virtual LoxValue Visit(GroupingExpr &expr);
  virtual LoxValue Visit(NullLiteralExpr &expr);
  virtual LoxValue Visit(BoolLiteralExpr &expr);
  virtual LoxValue Visit(VariableExpr &expr);
  virtual void Visit(ExprStmt &stmt);
  virtual void Visit(PrintStmt &stmt);
  virtual void Visit(VarStmt &stmt);

private:
};