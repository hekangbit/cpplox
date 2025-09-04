#include "interpreter.h"
#include "loxcallable.h"
#include "loxclass.h"
#include "loxinstance.h"

Interpreter::Interpreter() {
  global_env = new Environment();
  global_env->Define(
      "clock",
      make_shared<LoxCallable>(
          0,
          [](Interpreter &interpreter,
             const vector<Value> &arguments) -> Value {
            auto now = std::chrono::system_clock::now();
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                       now.time_since_epoch())
                       .count() /
                   1000.0;
          },
          "<native fn>"));
}

Value Interpreter::LookupVariable(token_t name, Expr *expr) {
  if (locals.count(expr)) {
    int depth = locals[expr];
    return cur_env->GetAt(depth, name->lexeme);
  }
  return global_env->Get(name);
}

bool Interpreter::IsTruthy(Value value) {
  if (value.isNil()) {
    return false;
  }
  if (value.isBool()) {
    return value.getBool();
  }
  return true;
}

void Interpreter::CheckNumOperand(const token_t op, const Value &value) {
  if (value.isDouble()) {
    return;
  }
  string tmp = op->lexeme;
  tmp.insert(0, "For operator <");
  tmp.append(">, Operand must be a number.");
  throw RuntimeException(op, tmp);
}

void Interpreter::CheckNumOperands(const token_t op, const Value &left,
                                   const Value &right) {
  if (left.isDouble() && right.isDouble()) {
    return;
  }
  string tmp = op->lexeme;
  tmp.insert(0, "For operator <");
  tmp.append(">, Operands must be a numbers.");
  throw RuntimeException(op, tmp);
}

Value Interpreter::Visit(NumberLiteralExpr &expr) {
  return Value(expr.num);
}

Value Interpreter::Visit(StringLiteralExpr &expr) {
  return Value(expr.str);
}

Value Interpreter::Visit(UnaryExpr &expr) {
  Value value = Evaluate(expr.right);
  if (expr.op->type == BANG) {
    return !IsTruthy(value);
  } else if (expr.op->type == MINUS) {
    CheckNumOperand(expr.op, value);
    return -(value.getDouble());
  }
  return Value();
}

Value Interpreter::Visit(BinaryExpr &expr) {
  Value leftValue = Evaluate(expr.left);
  Value rightValue = Evaluate(expr.right);
  Value result;

  switch (expr.op->type) {
  case STAR:
    result = leftValue * rightValue;
    break;
  case SLASH:
    result = leftValue / rightValue;
    break;
  case PLUS:
    result = leftValue + rightValue;
    break;
  case MINUS:
    result = leftValue - rightValue;
    break;
  case GREATER:
    result = leftValue > rightValue;
    break;
  case GREATER_EQUAL:
    result = leftValue >= rightValue;
    break;
  case LESS:
    result = leftValue < rightValue;
    break;
  case LESS_EQUAL:
    result = leftValue <= rightValue;
    break;
  case EQUAL_EQUAL:
    result = leftValue == rightValue;
    break;
  case BANG_EQUAL:
    result = leftValue != rightValue;
    break;
  default:
    break;
  }

  if (result.isNil()) {
    string tmp = expr.op->lexeme;
    tmp.insert(0, "Operator <");
    tmp.append("> with unmatch operand type.");
    throw RuntimeException(expr.op, tmp);
  }

  return result;
}

Value Interpreter::Visit(GroupingExpr &expr) {
  return Evaluate(expr.expr);
}

Value Interpreter::Visit(NullLiteralExpr &expr) {
  return Value();
}

Value Interpreter::Visit(BoolLiteralExpr &expr) {
  return Value(expr.val);
}

Value Interpreter::Visit(VariableExpr &expr) {
  return LookupVariable(expr.token, &expr);
}

Value Interpreter::Visit(LogicalExpr &expr) {
  Value leftVal = Evaluate(expr.left);
  if (expr.op->type == OR) {
    return (IsTruthy(leftVal)) ? leftVal : Evaluate(expr.right);
  } else if (expr.op->type == AND) {
    return (!IsTruthy(leftVal)) ? leftVal : Evaluate(expr.right);
  }
  throw RuntimeException(expr.op, "Invalid operator for logical expr.");
}

Value Interpreter::Visit(AssignExpr &expr) {
  Value value = Evaluate(expr.value);

  if (locals.count(&expr)) {
    int depth = locals[&expr];
    cur_env->AssignAt(depth, expr.name, value);
  } else {
    global_env->Assign(expr.name, value);
  }

  return value;
}

Value Interpreter::Visit(CallExpr &expr) {
  Value callee = Evaluate(expr.callee);
  vector<Value> arguments;

  if (!callee.isLoxCallable()) {
    throw RuntimeException(expr.paren, "Can only call functions and classes.");
  }

  for (auto arg : expr.arguments) {
    arguments.push_back(Evaluate(arg));
  }

  lox_callable_t func = callee.getLoxCallable();

  if (expr.arguments.size() != func->Arity()) {
    throw RuntimeException(expr.paren, "Expected " + to_string(func->Arity()) +
                                           " arguments but got " +
                                           to_string(expr.arguments.size()) +
                                           ".");
  }
  return func->Call(*this, arguments);
}

Value Interpreter::Visit(GetExpr &expr) {
  Value obj = Evaluate(expr.object);
  if (obj.isLoxInstance()) {
    auto instance = obj.getLoxInstance();
    return instance->Get(expr.name, instance);
  }
  throw RuntimeException(expr.name, "Only lox instance can get property.");
}

Value Interpreter::Visit(SetExpr &expr) {
  Value obj = Evaluate(expr.object);
  if (obj.isLoxInstance()) {
    Value val = Evaluate(expr.value);
    auto instance = obj.getLoxInstance();
    instance->Set(expr.name, val);
    return val;
  }
  throw RuntimeException(expr.name, "Only lox instance can set property.");
}

Value Interpreter::Visit(ThisExpr &expr) {
  return LookupVariable(expr.keyword, &expr);
}

Value Interpreter::Visit(SuperExpr &expr) {
  Value val = LookupVariable(expr.keyword, &expr);
  if (!(val.isLoxCallable() && dynamic_pointer_cast<LoxClass>(val.getLoxCallable()))) {
    throw RuntimeException(expr.keyword, "Super keyword not bind to class.");
  }

  auto superklass = dynamic_pointer_cast<LoxClass>(val.getLoxCallable());
  lox_func_t raw_method = superklass->FindMethod(expr.method->lexeme);
  if (raw_method == nullptr) {
    throw RuntimeException(expr.method,
      "Can't find method <" + expr.method->lexeme+ "> in superclass <" + superklass->name + ">.");
  }

  lox_instance_t instance;
  if (locals.count(&expr)) {
    int depth = locals[&expr];
    auto val = cur_env->GetAt(depth - 1, "this");
    if (!(val.isLoxInstance())) {
      throw RuntimeException(expr.keyword, "Can't find proper 'this' instance in super expr.");
    }
    instance = val.getLoxInstance();
  }
  if (instance == nullptr) {
    throw RuntimeException(expr.keyword, "Invalid instance when make lox function for method.");
  }
  auto method = raw_method->Bind(instance);
  lox_callable_t callable(method); // upcast
  return callable;
}

void Interpreter::Visit(ExprStmt &stmt) {
  Value value = Evaluate(stmt.expr);
}

void Interpreter::Visit(PrintStmt &stmt) {
  Value value = Evaluate(stmt.expr);
  cout << value << endl;
}

void Interpreter::Visit(BlockStmt &stmt) {
  auto env = new Environment(cur_env);
  Execute(stmt.statements, env);
}

void Interpreter::Visit(VarStmt &stmt) {
  Value value;
  if (stmt.initializer != nullptr) {
    value = Evaluate(stmt.initializer);
  }
  cur_env->Define(stmt.token->lexeme, value);
}

void Interpreter::Visit(IfStmt &stmt) {
  Value value = Evaluate(stmt.condition);
  if (IsTruthy(value)) {
    Execute(stmt.thenStmt);
  } else {
    Execute(stmt.elseStmt);
  }
}

void Interpreter::Visit(WhileStmt &stmt) {
  while (IsTruthy(Evaluate(stmt.condition))) {
    try {
      Execute(stmt.body);
    } catch (const RuntimeBreak &e) {
      break;
    }
  }
}

void Interpreter::Visit(BreakStmt &stmt) {
  throw RuntimeBreak();
}

void Interpreter::Visit(FunctionStmt &stmt) {
  shared_ptr<LoxCallable> function(new LoxFunction(stmt, cur_env, false)); // upcast
  cur_env->Define(stmt.name->lexeme, function);
}

void Interpreter::Visit(ReturnStmt &stmt) {
  Value val;
  if (stmt.expr) {
    val = Evaluate(stmt.expr);
  }
  throw RuntimeReturn(val);
}

void Interpreter::Visit(ClassStmt &stmt) {
  lox_class_t superclass;
  Value superclass_val;
  if (stmt.superclass) {
    superclass_val = Evaluate(stmt.superclass);
    if (!(superclass_val.isLoxCallable() && dynamic_pointer_cast<LoxClass>(superclass_val.getLoxCallable()))) {
      throw RuntimeException(stmt.superclass->token, "Superclass must be a class.");
    }
    superclass = dynamic_pointer_cast<LoxClass>(superclass_val.getLoxCallable());
  }

  cur_env->Define(stmt.name->lexeme, Value());

  if (stmt.superclass) {
    cur_env = new Environment(cur_env); // store "super" name
    cur_env->Define("super", superclass_val);
  }

  map<string, lox_func_t> methods;
  for (auto func_stmt : stmt.methods) {
    bool is_init = func_stmt->name->lexeme.compare("init") == 0;
    auto method = make_shared<LoxFunction>(*(func_stmt.get()), cur_env, is_init);
    methods[func_stmt->name->lexeme] = method;
  }

  lox_callable_t klass(new LoxClass(stmt.name->lexeme, superclass, methods)); // upcast

  if (stmt.superclass) {
    cur_env = cur_env->enclosing;
  }

  cur_env->Assign(stmt.name, klass);
}

Value Interpreter::Evaluate(expr_t expr) {
  return expr->Accept(*this);
}

void Interpreter::Execute(vector<stmt_t> statements, Environment *env) {
  Environment *prev_env = cur_env;
  cur_env = env;
  try {
    for (auto &statement : statements) {
      statement->Accept(*this);
    }
  } catch (const RuntimeException &e) {
    cout << "Catch Runtime exception in block." << endl;
    LoxRuntimeError(e.token->line, e.message);
  } catch (const RuntimeBreak &e) {
    cur_env = prev_env;
    throw RuntimeBreak();
  } catch (const RuntimeReturn &e) {
    cur_env = prev_env;
    throw RuntimeReturn(e);
  }

  cur_env = prev_env;
}

void Interpreter::Execute(stmt_t stmt) {
  if (stmt) {
    stmt->Accept(*this);
  }
}

void Interpreter::Interpret(vector<stmt_t> &statements) {
  cur_env = global_env;
  try {
    for (auto &statement : statements) {
      statement->Accept(*this);
    }
  } catch (const RuntimeException &e) {
    cout << "Catch Runtime exception." << endl;
    LoxRuntimeError(e.token->line, e.message);
  }
}

void Interpreter::Resolve(Expr *expr, int depth) {
  locals[expr] = depth;
}