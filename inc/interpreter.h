#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>
#include "common.h"
#include "visitor.h"
#include "value.h"
#include "stmt.h"
#include "expr.h"

class Interpreter : public Visitor {
public:
  Interpreter(vector<Stmt*> statements) : statements(statements) {}
  ~Interpreter() {}

  virtual LoxValue Visit(NumberLiteralExpr &expr);
  virtual LoxValue Visit(StringLiteralExpr &expr);
  virtual LoxValue Visit(UnaryExpr &expr);
  virtual LoxValue Visit(BinaryExpr &expr);
  virtual LoxValue Visit(GroupingExpr &expr);
  virtual LoxValue Visit(NullLiteralExpr &expr);
  virtual LoxValue Visit(BoolLiteralExpr &expr);
  virtual void Visit(ExprStmt &stmt);
  virtual void Visit(PrintStmt &stmt);
  virtual void Visit(VarStmt &stmt);

  LoxValue Evaluate(Expr &expr);
  void Execute();

private:
  vector<Stmt*> statements;
};

#endif