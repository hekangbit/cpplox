#include "environment.h"
#include "rtexception.h"

void Environment::Define(string name, const Value value) {
  values[name] = value;
}

void Environment::Assign(token_t token, const Value value) {
  if (values.count(token->lexeme)) {
    values[token->lexeme] = value;
    return;
  }
  if (enclosing != nullptr) {
    enclosing->Assign(token, value);
    return;
  }
  throw RuntimeException(token, string("Undefined variable <") + token->lexeme +
                                    "> in Assign Env.");
}

void Environment::AssignAt(int depth, token_t name, const Value value) {
  if (depth == 0) {
    Assign(name, value);
    return;
  }
  Environment *tmp_env = this;
  while (depth--) {
    tmp_env = tmp_env->enclosing;
  }
  tmp_env->Assign(name, value);
}

Value Environment::Get(token_t token) {
  if (values.count(token->lexeme)) {
    return values[token->lexeme];
  }
  if (enclosing != nullptr) {
    return enclosing->Get(token);
  }
  throw RuntimeException(token, string("Undefined variable <") + token->lexeme +
                                    "> in Get Env.");
}

Value Environment::GetAt(int depth, string name) {
  if (depth == 0) {
    return values[name];
  }
  Environment *tmp_env = this;
  while (depth--) {
    tmp_env = tmp_env->enclosing;
  }
  return tmp_env->values[name];
}