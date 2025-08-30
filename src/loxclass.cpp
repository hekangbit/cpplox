#include "loxclass.h"
#include "interpreter.h"
#include "loxinstance.h"

Value LoxClass::Call(Interpreter &interpreter, vector<Value> &arguments) {
  auto instance = make_shared<LoxInstance>(this);
  return Value(instance);
}