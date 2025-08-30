#ifndef LOXCLASS_H
#define LOXCLASS_H

#include "common.h"

class LoxClass {
public:
  LoxClass(string name) : name(name) {}

  string toString() {
    return name;
  }

  string name;
};

#endif