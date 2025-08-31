#include "loxinstance.h"
#include "rtexception.h"
#include "value.h"

Value LoxInstance::Get(token_t name) {
  if (fields.count(name->lexeme)) {
    return fields[name->lexeme];
  }
  throw RuntimeException(name, "Underfined property '" + name->lexeme + "'.");
}

void LoxInstance::Set(token_t name, Value val) {
  fields[name->lexeme] = val;
}