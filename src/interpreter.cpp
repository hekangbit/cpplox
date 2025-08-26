#include "interpreter.h"
#include "loxcallable.h"

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
  throw RuntimeException(token, string("Undefined variable <") + token->lexeme + "> .");
}

Value Environment::Get(token_t token) {
  if (values.count(token->lexeme)) {
    return values[token->lexeme];
  }
  if (enclosing != nullptr) {
    return enclosing->Get(token);
  }
  throw RuntimeException(token, string("Undefined variable <") + token->lexeme + "> .");
}

Value LoxFunction::Call(Interpreter &interpreter, vector<Value> &arguments) {
  unique_ptr<Environment> env = make_unique<Environment>(&(interpreter.global_env));

  for (int i = 0; i < declaration.params.size(); i++) {
    env->Define(declaration.params[i]->lexeme, arguments[i]);
  }
  auto body = dynamic_pointer_cast<BlockStmt>(declaration.body);
  if (!body) {
    throw RuntimeException(declaration.name,  "Expect block stmt body for function.");
  }
  interpreter.Execute(body->statements, env.get());
  return Value();
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

void Interpreter::CheckNumOperands(const token_t op, const Value &left, const Value &right) {
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

Value Interpreter::Visit(NullLiteralExpr &expr) { return Value(); }

Value Interpreter::Visit(BoolLiteralExpr &expr) {
  return Value(expr.val);
}

Value Interpreter::Visit(VariableExpr &expr) {
  return cur_env->Get(expr.token);
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
  cur_env->Assign(expr.name, value);
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
    throw RuntimeException(expr.paren,  "Expected " + to_string(func->Arity()) +
      " arguments but got " + to_string(expr.arguments.size()) + ".");
  }
  return func->Call(*this, arguments);
}

void Interpreter::Visit(ExprStmt &stmt) {
  Value value = Evaluate(stmt.expr);
}

void Interpreter::Visit(PrintStmt &stmt) {
  Value value = Evaluate(stmt.expr);
  cout << value << endl;
}

void Interpreter::Visit(BlockStmt &stmt) {
  auto env = make_unique<Environment>(cur_env);
  Execute(stmt.statements, env.get());
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
  shared_ptr<LoxCallable> function(new LoxFunction(stmt));
  cur_env->Define(stmt.name->lexeme, function);
}

Value Interpreter::Evaluate(expr_t expr) { return expr->Accept(*this); }

void Interpreter::Execute(vector<stmt_t> statements, Environment *env) {
  Environment *prev_env = cur_env;
  cur_env = env;
  try {
    for (auto &statement : statements) {
      statement->Accept(*this);
    }
  } catch (const RuntimeException &e) {
    cout << "Catch Runtime exception in block." << endl;
    runtimeError(e.token->line, e.message);
  } catch (const RuntimeBreak &e) {
    cur_env = prev_env;
    throw RuntimeBreak();
  }
  cur_env = prev_env;
}

void Interpreter::Execute(stmt_t stmt) {
  if (stmt) {
    stmt->Accept(*this);
  }
}

void Interpreter::Execute() {
  cur_env = &global_env;
  try {
    for (auto &statement : statements) {
      statement->Accept(*this);
    }
  } catch (const RuntimeException &e) {
    cout << "Catch Runtime exception." << endl;
    runtimeError(e.token->line, e.message);
  }
}