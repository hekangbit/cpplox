#ifndef RESOLVER_H
#define RESOLVER_H

#include "common.h"
#include "expr.h"
#include "interpreter.h"
#include "loxerror.h"
#include "stmt.h"
#include "visitor.h"

class Resolver : public Visitor {
public:
  Resolver(Interpreter *interpreter)
      : interpreter(interpreter), is_func_enclosing(false) {}

  void Declare(token_t token);
  void Define(token_t token);
  void BeginScope();
  void EndScope();
  void Resolve(expr_t expr);
  void Resolve(stmt_t statement);
  void Resolve(vector<stmt_t> &statements);
  void ResolveLocal(Expr *expr, token_t token);
  void ResolveFunction(FunctionStmt &func);
  void ResolveClass(ClassStmt &klass);

  virtual Value Visit(NumberLiteralExpr &expr);
  virtual Value Visit(StringLiteralExpr &expr);
  virtual Value Visit(UnaryExpr &expr);
  virtual Value Visit(BinaryExpr &expr);
  virtual Value Visit(GroupingExpr &expr);
  virtual Value Visit(NullLiteralExpr &expr);
  virtual Value Visit(BoolLiteralExpr &expr);
  virtual Value Visit(LogicalExpr &expr);
  virtual Value Visit(VariableExpr &expr);
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

  vector<map<string, bool>> scopes;
  Interpreter *interpreter;
  bool is_func_enclosing;
};

#endif