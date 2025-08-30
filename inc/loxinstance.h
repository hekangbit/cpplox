#ifndef LOXINSTANCE_H
#define LOXINSTANCE_H

#include "common.h"
#include "loxclass.h"

class LoxInstance {
public:
  LoxInstance(LoxClass *klass) : klass(klass) {}
  string toString() const {
    return klass->name + " instance";
  }

  LoxClass *klass;
};

#endif