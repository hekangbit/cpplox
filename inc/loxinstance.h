#ifndef LOXINSTANCE_H
#define LOXINSTANCE_H

#include "common.h"
#include "loxclass.h"
#include "token.h"

class LoxInstance {
public:
  LoxInstance(LoxClass *klass) : klass(klass) {}
  string toString() const {
    return klass->name + " instance";
  }

  Value Get(token_t name);
  void Set(token_t name, Value val);

  map<string, Value> fields;
  LoxClass *klass;
};

#endif