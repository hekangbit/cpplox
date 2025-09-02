#ifndef LOXINSTANCE_H
#define LOXINSTANCE_H

#include "common.h"
#include "token.h"
#include "loxclass.h"

class LoxInstance {
public:
  LoxInstance(LoxClass *klass) : klass(klass) {}
  string toString() const {
    return klass->name + " instance";
  }

  Value Get(token_t name, lox_instance_t &instance);
  void Set(token_t name, Value val);

  map<string, Value> fields;
  LoxClass *klass;
};

#endif