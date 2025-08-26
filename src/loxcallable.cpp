#include "loxcallable.h"
#include "value.h"

Value LoxCallable::Call(Interpreter &interpreter, vector<Value> &arguments) {
  return func(interpreter, arguments);
}