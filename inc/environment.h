#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "common.h"
#include "token.h"
#include "value.h"

class Environment {
public:
  Environment() : enclosing(nullptr) {}
  Environment(Environment *enclosing) : enclosing(enclosing) {
    if (enclosing) {
      enclosing->Add(this);
    }
  }
  void Define(string name, const Value value);
  void Assign(token_t token, const Value value);
  void AssignAt(int depth, token_t token, const Value value);
  Value Get(token_t token);
  Value GetAt(int depth, string name);
  void Add(Environment *env) {
    childs.push_back(env);
  }

  unordered_map<string, Value> values;
  list<Environment *> childs;
  Environment *enclosing;
};

#endif