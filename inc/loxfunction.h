#ifndef LOXFUNCTION_H
#define LOXFUNCTION_H

#include "common.h"
#include "value.h"
#include "loxcallable.h"
#include "stmt.h"
#include "environment.h"

class Interpreter;
class LoxFunction : public LoxCallable {
public:
  LoxFunction(FunctionStmt &declaration, environment_t env)
      : declaration(declaration), closure(env) {}

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

using lox_func_t = shared_ptr<LoxFunction>;

#endif