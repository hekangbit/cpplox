#include "loxclass.h"
#include "interpreter.h"
#include "loxinstance.h"

LoxClass::LoxClass(string name, lox_class_t superclass, map<string, lox_func_t> methods) :
  name(name), superclass(superclass), methods(methods) {}

int LoxClass::Arity() const {
  auto it = methods.find("init");
  if (it != methods.end()) {
    return it->second->Arity();
  }
  return 0;
}
string LoxClass::toString() const {
  return name;
}

Value LoxClass::Call(Interpreter &interpreter, vector<Value> &arguments) {
  auto instance = make_shared<LoxInstance>(this);
  auto it = methods.find("init");
  if (it != methods.end()) {
    auto ctor = it->second->Bind(instance);
    ctor->Call(interpreter, arguments); // run construct
  }
  return Value(instance);
}