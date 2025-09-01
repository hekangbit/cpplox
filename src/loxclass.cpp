#include "loxclass.h"
#include "interpreter.h"
#include "loxinstance.h"

LoxClass::LoxClass(string name, map<string, lox_func_t> methods) : name(name), methods(methods) {}

int LoxClass::Arity() const {
  return 0;
}
string LoxClass::toString() const {
  return name;
}

Value LoxClass::Call(Interpreter &interpreter, vector<Value> &arguments) {
  auto instance = make_shared<LoxInstance>(this);
  return Value(instance);
}