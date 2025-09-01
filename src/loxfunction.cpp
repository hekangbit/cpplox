#include "loxfunction.h"
#include "interpreter.h"

Value LoxFunction::Call(Interpreter &interpreter, vector<Value> &arguments) {
  auto env = make_shared<Environment>(closure);

  for (int i = 0; i < declaration.params.size(); i++) {
    env->Define(declaration.params[i]->lexeme, arguments[i]);
  }

  try {
    interpreter.Execute(declaration.body, env);
  } catch (const RuntimeReturn &e) {
    return e.val;
  }

  return Value();
}