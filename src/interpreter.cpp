#include "interpreter.h"

void Enviroment::Define(Token &name, const Value value) {
  values[name.lexeme] = value;
}

void Enviroment::Assign(Token &name, const Value value) {
  if (values.count(name.lexeme)) {
    values[name.lexeme] = value;
    return;
  }
  if (enclosing != nullptr) {
    enclosing->Assign(name, value);
  }
  throw RuntimeException(name, string("Undefined variable ") + name.lexeme + " .");
}

Value Enviroment::Get(Token &name) {
  if (values.count(name.lexeme)) {
    return values[name.lexeme];
  }
  if (enclosing != nullptr) {
    return enclosing->Get(name);
  }
  throw RuntimeException(name, string("Undefined variable ") + name.lexeme + " .");
}

bool Interpreter::IsTruthy(Value &value) {
  if (value.isNil()) {
    return false;
  }
  if (value.isBool()) {
    return value.getBool();
  }
  return true;
}

Value Interpreter::Visit(NumberLiteralExpr &expr) {
  return Value(expr.num);
}

Value Interpreter::Visit(StringLiteralExpr &expr) {
  return Value(expr.str);
}

Value Interpreter::Visit(UnaryExpr &expr) {
  Value value = Evaluate(expr.right);
  if (expr.op.type == BANG) {
    if (value.isBool()) {
      return !value.getBool();
    }
  } else if (expr.op.type == MINUS) {
    if (value.isDouble()) {
      return -(value.getDouble());
    }
  }
  return Value();
}

Value Interpreter::Visit(BinaryExpr &expr) {
  Value leftValue = Evaluate(expr.left);
  Value rightValue = Evaluate(expr.right);
  Value result;

  switch (expr.op.type) {
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
  default:
    break;
  }

  if (result.isNil()) {
    throw RuntimeException(expr.op, "binary expr has unmatch value type");
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
  return cur_env->Get(expr.var);
}

Value Interpreter::Visit(AssignExpr &expr) {
  Value value = Evaluate(expr.value);
  cur_env->Assign(expr.name, value);
  return value;
}

void Interpreter::Visit(ExprStmt &stmt) {
  Value value = Evaluate(stmt.expr);
}

void Interpreter::Visit(PrintStmt &stmt) {
  Value value = Evaluate(stmt.expr);
  cout << value << endl;
}

void Interpreter::Visit(BlockStmt &stmt) {
  ExecuteBlock(stmt.statements, cur_env);
}

void Interpreter::Visit(VarStmt &stmt) {
  Value value;
  if (stmt.initializer != nullptr) {
    value = Evaluate(stmt.initializer);
  }
  cur_env->Define(*(stmt.token), value);
}

Value Interpreter::Evaluate(Expr *expr) { return expr->Accept(*this); }

void Interpreter::ExecuteBlock(list<Stmt*> statements, Enviroment *env) {
  Enviroment *prev_env = env;
  cur_env = new Enviroment(env);
  try {
    for (auto &statement : statements) {
      statement->Accept(*this);
    }
  } catch (const RuntimeException &e) {
    cout << "Catch Runtime exception in block" << endl;
    error(e.token.line, e.message);
    cerr << e.what() << endl;
  }
  delete cur_env;
  cur_env = prev_env;
}

void Interpreter::Execute() {
  cur_env = &global_env;
  try {
    for (auto &statement : statements) {
      statement->Accept(*this);
    }
  } catch (const RuntimeException &e) {
    cout << "Catch Runtime exception" << endl;
    error(e.token.line, e.message);
    cerr << e.what() << endl;
  }
}