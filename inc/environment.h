#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "common.h"
#include "token.h"
#include "value.h"

class Environment {
public:
  using env_t = shared_ptr<Environment>;
  Environment() {}
  Environment(env_t enclosing) : enclosing(enclosing) {}
  void Define(string name, const Value value);
  void Assign(token_t token, const Value value);
  void AssignAt(int depth, token_t token, const Value value);
  Value Get(token_t token);
  Value GetAt(int depth, string name);

private:
  unordered_map<string, Value> values;
  env_t enclosing;
};

using environment_t = Environment::env_t;

#endif