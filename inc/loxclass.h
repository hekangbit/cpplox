#ifndef LOXCLASS_H
#define LOXCLASS_H

#include "common.h"
#include "value.h"
#include "loxcallable.h"
#include "loxfunction.h"

class LoxClass : public LoxCallable {
public:
  LoxClass(string name, map<string, lox_func_t> methods);
  virtual Value Call(Interpreter &interpreter, vector<Value> &arguments);
  virtual int Arity() const;
  virtual string toString() const;
  string name;
  map<string, lox_func_t> methods;
};

#endif