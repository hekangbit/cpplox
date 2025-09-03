#include "loxinstance.h"
#include "rtexception.h"
#include "value.h"

Value LoxInstance::Get(token_t name, lox_instance_t &instance) {
  if (fields.count(name->lexeme)) {
    return fields[name->lexeme];
  }
  if (klass->methods.count(name->lexeme)) {
    lox_callable_t p(klass->methods[name->lexeme]->Bind(instance));
    return p;
  }
  throw RuntimeException(name, "Undefined property '" + name->lexeme + "'.");
}

void LoxInstance::Set(token_t name, Value val) {
  fields[name->lexeme] = val;
}