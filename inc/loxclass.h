#ifndef LOXCLASS_H
#define LOXCLASS_H

#include "common.h"
#include "value.h"
#include "loxcallable.h"
#include "loxfunction.h"

class LoxClass;
using lox_class_t = shared_ptr<LoxClass>;

class LoxClass : public LoxCallable {
public:
  LoxClass(string name, lox_class_t superclass, map<string, lox_func_t> methods);
  virtual Value Call(Interpreter &interpreter, vector<Value> &arguments);
  virtual int Arity() const;
  virtual string toString() const;
  string name;
  lox_class_t superclass;
  map<string, lox_func_t> methods;
};

#endif