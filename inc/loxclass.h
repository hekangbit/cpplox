#ifndef LOXCLASS_H
#define LOXCLASS_H

#include "common.h"
#include "loxcallable.h"
#include "value.h"

class LoxClass : public LoxCallable {
public:
  LoxClass(string name) : name(name) {}
  virtual Value Call(Interpreter &interpreter, vector<Value> &arguments);
  virtual int Arity() const {
    return 0;
  }
  virtual string toString() const {
    return name;
  }

  string name;
};

#endif