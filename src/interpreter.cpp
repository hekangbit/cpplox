#include "interpreter.h"

void Enviroment::Define(token_t name, const Value value) {
  values[name->lexeme] = value;
}

void Enviroment::Assign(token_t name, const Value value) {
  if (values.count(name->lexeme)) {
    values[name->lexeme] = value;
    return;
  }
  if (enclosing != nullptr) {
    enclosing->Assign(name, value);
    return;
  }
  throw RuntimeException(name, string("Undefined variable <") + name->lexeme + "> .");
}

Value Enviroment::Get(token_t name) {
  if (values.count(name->lexeme)) {
    return values[name->lexeme];
  }
  if (enclosing != nullptr) {
    return enclosing->Get(name);
  }
  throw RuntimeException(name, string("Undefined variable <") + name->lexeme + "> .");
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
  cout << "Run CallExpr" << endl;
  return Value();
}

void Interpreter::Visit(ExprStmt &stmt) {
  Value value = Evaluate(stmt.expr);
}

void Interpreter::Visit(PrintStmt &stmt) {
  Value value = Evaluate(stmt.expr);
  cout << value << endl;
}

void Interpreter::Visit(BlockStmt &stmt) {
  Execute(stmt.statements, cur_env);
}

void Interpreter::Visit(VarStmt &stmt) {
  Value value;
  if (stmt.initializer != nullptr) {
    value = Evaluate(stmt.initializer);
  }
  cur_env->Define(stmt.token, value);
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

Value Interpreter::Evaluate(expr_t expr) { return expr->Accept(*this); }

void Interpreter::Execute(vector<stmt_t> statements, Enviroment *env) {
  Enviroment *prev_env = env;
  cur_env = new Enviroment(env);
  try {
    for (auto &statement : statements) {
      statement->Accept(*this);
    }
  } catch (const RuntimeException &e) {
    cout << "Catch Runtime exception in block." << endl;
    runtimeError(e.token->line, e.message);
  } catch (const RuntimeBreak &e) {
    delete cur_env;
    cur_env = prev_env;
    throw RuntimeBreak();
  }
  delete cur_env;
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