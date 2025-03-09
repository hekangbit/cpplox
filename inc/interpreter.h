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
  void Define(Token &name, const LoxValue value);
  void Assign(Token &name, const LoxValue value);
  LoxValue Get(Token &name);

private:
  unordered_map<string, LoxValue> values;
  Enviroment *enclosing;
};

class Interpreter : public Visitor {
public:
  Interpreter(vector<Stmt *> statements) : statements(statements) {}
  ~Interpreter() {}

  virtual LoxValue Visit(NumberLiteralExpr &expr);
  virtual LoxValue Visit(StringLiteralExpr &expr);
  virtual LoxValue Visit(UnaryExpr &expr);
  virtual LoxValue Visit(BinaryExpr &expr);
  virtual LoxValue Visit(GroupingExpr &expr);
  virtual LoxValue Visit(NullLiteralExpr &expr);
  virtual LoxValue Visit(BoolLiteralExpr &expr);
  virtual LoxValue Visit(VariableExpr &expr);
  virtual LoxValue Visit(AssignExpr &expr);
  virtual void Visit(ExprStmt &stmt);
  virtual void Visit(PrintStmt &stmt);
  virtual void Visit(BlockStmt &stmt);
  virtual void Visit(VarStmt &stmt);

  LoxValue Evaluate(Expr *expr);
  void ExecuteBlock(list<Stmt*> statements, Enviroment *env);
  void Execute();

private:
  vector<Stmt *> statements;
  Enviroment global_env;
  Enviroment *cur_env;
};

#endif