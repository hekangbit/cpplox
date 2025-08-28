#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "common.h"
#include "error.h"
#include "expr.h"
#include "loxcallable.h"
#include "stmt.h"
#include "value.h"
#include "visitor.h"
#include <exception>
#include <unordered_map>
#include <vector>

class Interpreter;

class RuntimeException : public exception {
public:
  RuntimeException(token_t token, string message)
      : token(token), message(message) {}
  const char *what() const throw() {
    return message.c_str();
  }
  token_t token;
  string message;
};

class RuntimeBreak : public exception {
public:
};

class RuntimeReturn : public exception {
public:
  RuntimeReturn(Value val) : val(val) {}
  Value val;
};


class Environment {
public:
  using env_t = shared_ptr<Environment>;
  Environment() : enclosing(nullptr) {}
  Environment(env_t enclosing) : enclosing(enclosing) {}
  void Define(string name, const Value value);
  void Assign(token_t token, const Value value);
  Value Get(token_t token);

private:
  unordered_map<string, Value> values;
  env_t enclosing;
};

using environment_t = Environment::env_t;

class LoxFunction : public LoxCallable {
public:
  LoxFunction(FunctionStmt &declaration, environment_t env) : declaration(declaration), closure(env) {}

  virtual int Arity() const {
    return declaration.params.size();
  }
  virtual Value Call(Interpreter &interpreter, vector<Value> &arguments);
  virtual string toString() const {
    return "<fn " + declaration.name->lexeme + ">";
  }

  virtual ~LoxFunction() {}

private:
  FunctionStmt declaration;
  environment_t closure;
};

class Interpreter : public Visitor {
public:
  Interpreter(vector<stmt_t> statements) : statements(statements) {
    global_env = make_shared<Environment>();
    global_env->Define(
        "clock",
        make_shared<LoxCallable>(
            0,
            [](Interpreter &interpreter,
               const vector<Value> &arguments) -> Value {
              auto now = std::chrono::system_clock::now();
              return std::chrono::duration_cast<std::chrono::milliseconds>(
                         now.time_since_epoch())
                         .count() /
                     1000.0;
            },
            "<native fn>"));
  }
  ~Interpreter() {}

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
  virtual void Visit(ExprStmt &stmt);
  virtual void Visit(PrintStmt &stmt);
  virtual void Visit(BlockStmt &stmt);
  virtual void Visit(VarStmt &stmt);
  virtual void Visit(IfStmt &stmt);
  virtual void Visit(WhileStmt &stmt);
  virtual void Visit(BreakStmt &stmt);
  virtual void Visit(FunctionStmt &stmt);
  virtual void Visit(ReturnStmt &stmt);

  Value Evaluate(expr_t expr);
  void Execute(vector<stmt_t> statements, environment_t env);
  void Execute(stmt_t stmt);
  void Execute();

  environment_t cur_env;
  environment_t global_env;

private:
  vector<stmt_t> statements;
};

#endif