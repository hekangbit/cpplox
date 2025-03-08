#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <exception>
#include <vector>
#include <unordered_map>
#include "common.h"
#include "error.h"
#include "visitor.h"
#include "value.h"
#include "stmt.h"
#include "expr.h"



class RuntimeException : public exception {
public:
  RuntimeException(Token token, string message) : token(token), message(message) {}
  const char *what() const throw() { return message.c_str(); }
  Token token;
  string message;
};

class Enviroment {
public:
  void Define(string name, const LoxValue value);
  LoxValue Get(string name);

private:
  unordered_map<string, LoxValue> values;
};

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
  virtual LoxValue Visit(VariableExpr &expr);
  virtual void Visit(ExprStmt &stmt);
  virtual void Visit(PrintStmt &stmt);
  virtual void Visit(VarStmt &stmt);

  LoxValue Evaluate(Expr &expr);
  void Execute();

private:
  vector<Stmt*> statements;
  Enviroment global_env;
};

#endif