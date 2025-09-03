#include "loxinstance.h"
#include "rtexception.h"
#include "value.h"

Value LoxInstance::Get(token_t name, lox_instance_t &instance) {
  if (fields.count(name->lexeme)) {
    return fields[name->lexeme];
  }
  auto method = klass->FindMethod(name->lexeme);
  if (method) {
    lox_callable_t p(method->Bind(instance));
    return p;
  }
  throw RuntimeException(name, "Undefined property '" + name->lexeme + "'.");
}

void LoxInstance::Set(token_t name, Value val) {
  fields[name->lexeme] = val;
}