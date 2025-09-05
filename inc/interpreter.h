#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "common.h"
#include "environment.h"
#include "expr.h"
#include "loxerror.h"
#include "loxfunction.h"
#include "rtexception.h"
#include "stmt.h"
#include "value.h"
#include "visitor.h"

class RuntimeBreak : public exception {
public:
};

class RuntimeReturn : public exception {
public:
  RuntimeReturn(Value val) : val(val) {}
  Value val;
};

class Interpreter : public Visitor {
public:
  Interpreter();
  ~Interpreter() {}

  Value LookupVariable(token_t name, Expr *expr);
  bool IsTruthy(Value value);
  void CheckNumOperand(const token_t op, const Value &value);
  void CheckNumOperands(const token_t op, const Value &left,
                        const Value &right);

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
  virtual Value Visit(GetExpr &expr);
  virtual Value Visit(SetExpr &expr);
  virtual Value Visit(ThisExpr &expr);
  virtual Value Visit(SuperExpr &expr);
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

  Value Evaluate(expr_t expr);
  void Execute(vector<stmt_t> statements, Environment *env);
  void Execute(stmt_t stmt);
  void Interpret(vector<stmt_t> &statements);

  void Resolve(Expr *expr, int depth);

  Environment *cur_env;
  Environment *global_env;
  unordered_map<Expr *, int> locals;
};

#endif