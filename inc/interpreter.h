#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "common.h"
#include "error.h"
#include "expr.h"
#include "stmt.h"
#include "value.h"
#include "visitor.h"
#include <exception>
#include <unordered_map>
#include <vector>

class RuntimeException : public exception {
public:
  RuntimeException(token_t token, string message)
      : token(token), message(message) {}
  const char *what() const throw() { return message.c_str(); }
  token_t token;
  string message;
};

class RuntimeBreak : public exception {
public:
};

class Enviroment {
public:
  Enviroment() : enclosing(nullptr) {}
  Enviroment(Enviroment *enclosing) : enclosing(enclosing) {}
  void Define(token_t name, const Value value);
  void Assign(token_t name, const Value value);
  Value Get(token_t name);

private:
  unordered_map<string, Value> values;
  Enviroment *enclosing;
};

class Interpreter : public Visitor {
public:
  Interpreter(vector<stmt_t> statements) : statements(statements) {}
  ~Interpreter() {}

  bool IsTruthy(Value value);
  void CheckNumOperand(const token_t op, const Value &value);
  void CheckNumOperands(const token_t op, const Value &left, const Value &right);

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
  virtual void Visit(ExprStmt &stmt);
  virtual void Visit(PrintStmt &stmt);
  virtual void Visit(BlockStmt &stmt);
  virtual void Visit(VarStmt &stmt);
  virtual void Visit(IfStmt &stmt);
  virtual void Visit(WhileStmt &stmt);
  virtual void Visit(BreakStmt &stmt);

  Value Evaluate(expr_t expr);
  void Execute(vector<stmt_t> statements, Enviroment *env);
  void Execute(stmt_t stmt);
  void Execute();

private:
  vector<stmt_t> statements;
  Enviroment global_env;
  Enviroment *cur_env;
};

#endif