#include "interpreter.h"

void Enviroment::Define(Token &name, const LoxValue value) {
  values[name.lexeme] = value;
}

void Enviroment::Assign(Token &name, const LoxValue value) {
  if (values.count(name.lexeme)) {
    values[name.lexeme] = value;
    return;
  }
  if (enclosing != nullptr) {
    enclosing->Assign(name, value);
  }
  throw RuntimeException(name, string("Undefined variable ") + name.lexeme + " .");
}

LoxValue Enviroment::Get(Token &name) {
  if (values.count(name.lexeme)) {
    return values[name.lexeme];
  }
  if (enclosing != nullptr) {
    return enclosing->Get(name);
  }
  throw RuntimeException(name, string("Undefined variable ") + name.lexeme + " .");
}

LoxValue Interpreter::Visit(NumberLiteralExpr &expr) {
  return LoxValue(expr.num);
}

LoxValue Interpreter::Visit(StringLiteralExpr &expr) {
  return LoxValue(expr.str);
}

LoxValue Interpreter::Visit(UnaryExpr &expr) {
  LoxValue value = Evaluate(expr.right);
  switch (expr.op.type) {
  case MINUS:
    return LoxValue(-(value.GetNum()));
  }
  return LoxValue();
}

LoxValue Interpreter::Visit(BinaryExpr &expr) {
  LoxValue leftValue = Evaluate(expr.left);
  LoxValue rightValue = Evaluate(expr.right);
  if (leftValue.IsNum() && rightValue.IsNum()) {
    switch (expr.op.type) {
    case STAR:
      return LoxValue(leftValue.GetNum() * rightValue.GetNum());
    case SLASH:
      return LoxValue(leftValue.GetNum() / rightValue.GetNum());
    case PLUS:
      return LoxValue(leftValue.GetNum() + rightValue.GetNum());
    case MINUS:
      return LoxValue(leftValue.GetNum() - rightValue.GetNum());
    default:
      break;
    }
  }
  throw RuntimeException(expr.op, "binary expr has unmatch value type");
}

LoxValue Interpreter::Visit(GroupingExpr &expr) {
  return Evaluate(expr.expr);
}

LoxValue Interpreter::Visit(NullLiteralExpr &expr) { return LoxValue(); }

LoxValue Interpreter::Visit(BoolLiteralExpr &expr) {
  if (expr.val) {
    return LoxValue(string("true"));
  }
  return LoxValue(string("false"));
}

LoxValue Interpreter::Visit(VariableExpr &expr) {
  return cur_env->Get(expr.var);
}

LoxValue Interpreter::Visit(AssignExpr &expr) {
  LoxValue value = Evaluate(expr.value);
  cur_env->Assign(expr.name, value);
  return value;
}

void Interpreter::Visit(ExprStmt &stmt) {
  LoxValue value = Evaluate(stmt.expr);
}

void Interpreter::Visit(PrintStmt &stmt) {
  LoxValue value = Evaluate(stmt.expr);
  if (value.IsNil()) {
    cout << "nil" << endl;
  } else if (value.IsNum()) {
    cout << value.GetNum() << endl;
  } else {
    cout << value.GetStr() << endl;
  }
}

void Interpreter::Visit(BlockStmt &stmt) {
  ExecuteBlock(stmt.statements, cur_env);
}

void Interpreter::Visit(VarStmt &stmt) {
  LoxValue value;
  if (stmt.initializer != nullptr) {
    value = Evaluate(stmt.initializer);
  }
  cur_env->Define(*(stmt.token), value);
}

LoxValue Interpreter::Evaluate(Expr *expr) { return expr->Accept(*this); }

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