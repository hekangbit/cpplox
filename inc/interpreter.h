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
  RuntimeException(Token token, string message)
      : token(token), message(message) {}
  const char *what() const throw() { return message.c_str(); }
  Token token;
  string message;
};

class Enviroment {
public:
  Enviroment() : enclosing(nullptr) {}
  Enviroment(Enviroment *enclosing) : enclosing(enclosing) {}
  void Define(Token &name, const Value value);
  void Assign(Token &name, const Value value);
  Value Get(Token &name);

private:
  unordered_map<string, Value> values;
  Enviroment *enclosing;
};

class Interpreter : public Visitor {
public:
  Interpreter(vector<Stmt *> statements) : statements(statements) {}
  ~Interpreter() {}

  bool IsTruthy(Value &value);
  virtual Value Visit(NumberLiteralExpr &expr);
  virtual Value Visit(StringLiteralExpr &expr);
  virtual Value Visit(UnaryExpr &expr);
  virtual Value Visit(BinaryExpr &expr);
  virtual Value Visit(GroupingExpr &expr);
  virtual Value Visit(NullLiteralExpr &expr);
  virtual Value Visit(BoolLiteralExpr &expr);
  virtual Value Visit(VariableExpr &expr);
  virtual Value Visit(AssignExpr &expr);
  virtual void Visit(ExprStmt &stmt);
  virtual void Visit(PrintStmt &stmt);
  virtual void Visit(BlockStmt &stmt);
  virtual void Visit(VarStmt &stmt);

  Value Evaluate(Expr *expr);
  void ExecuteBlock(list<Stmt*> statements, Enviroment *env);
  void Execute();

private:
  vector<Stmt *> statements;
  Enviroment global_env;
  Enviroment *cur_env;
};

#endif