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
  string Parenthesize(string &name, vector<expr_t> &exprs);
  virtual Value Visit(NumberLiteralExpr &expr);
  virtual Value Visit(StringLiteralExpr &expr);
  virtual Value Visit(UnaryExpr &expr);
  virtual Value Visit(BinaryExpr &expr);
  virtual Value Visit(GroupingExpr &expr);
  virtual Value Visit(NullLiteralExpr &expr);
  virtual Value Visit(BoolLiteralExpr &expr);
  virtual Value Visit(VariableExpr &expr);
  virtual Value Visit(LogicalExpr &expr);
  virtual Value Visit(AssignExpr &expr);
  virtual Value Visit(CallExpr &expr);
  virtual void Visit(ExprStmt &stmt);
  virtual void Visit(PrintStmt &stmt);
  virtual void Visit(BlockStmt &stmt);
  virtual void Visit(VarStmt &stmt);
  virtual void Visit(IfStmt &stmt);
  virtual void Visit(WhileStmt &stmt);
  virtual void Visit(BreakStmt &stmt);
  virtual void Visit(FunctionStmt &stmt);
  virtual void Visit(ReturnStmt &stmt);
  virtual void Visit(ClassStmt &stmt);

private:
};