#ifndef LOXFUNCTION_H
#define LOXFUNCTION_H

#include "common.h"
#include "stmt.h"
#include "value.h"
#include "environment.h"
#include "loxcallable.h"

class Interpreter;
class LoxFunction;

using lox_func_t = shared_ptr<LoxFunction>;

class LoxFunction : public LoxCallable {
public:
  LoxFunction(FunctionStmt &declaration, Environment *env, bool is_initializer)
      : declaration(declaration), closure(env), is_initializer(is_initializer) {}

  virtual int Arity() const {
    return declaration.params.size();
  }
  virtual Value Call(Interpreter &interpreter, vector<Value> &arguments);
  virtual string toString() const {
    return "<fn " + declaration.name->lexeme + ">";
  }
  virtual lox_func_t Bind(lox_instance_t &instance);

  virtual ~LoxFunction() {}

  FunctionStmt declaration;
  Environment *closure;
  bool is_initializer;
};



#endif