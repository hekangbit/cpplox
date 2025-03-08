#include "interpreter.h"

void Enviroment::Define(string name, const LoxValue value) {
  values[name] = value;
}

LoxValue Enviroment::Get(string name) {
  if (values.count(name)) {
    return values[name];
  }
  return LoxValue();
}

LoxValue Interpreter::Visit(NumberLiteralExpr &expr) {
  return LoxValue(expr.num);
}

LoxValue Interpreter::Visit(StringLiteralExpr &expr) {
  return LoxValue(expr.str);
}

LoxValue Interpreter::Visit(UnaryExpr &expr) {
  LoxValue value = Evaluate(*(expr.right));
  switch (expr.op.type) {
  case MINUS:
    return LoxValue(-(value.GetNum()));
  }
  return LoxValue();
}

LoxValue Interpreter::Visit(BinaryExpr &expr) {
  LoxValue leftValue = Evaluate(*(expr.left));
  LoxValue rightValue = Evaluate(*(expr.right));
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
  return Evaluate(*(expr.expr));
}

LoxValue Interpreter::Visit(NullLiteralExpr &expr) { return LoxValue(); }

LoxValue Interpreter::Visit(BoolLiteralExpr &expr) {
  if (expr.val) {
    return LoxValue(string("true"));
  }
  return LoxValue(string("false"));
}

LoxValue Interpreter::Visit(VariableExpr &expr) {
  return global_env.Get(expr.var.lexeme);
}

void Interpreter::Visit(ExprStmt &stmt) {
  LoxValue value = Evaluate(*(stmt.expr));
}

void Interpreter::Visit(PrintStmt &stmt) {
  LoxValue value = Evaluate(*(stmt.expr));
  if (value.IsNil()) {
    cout << "nil" << endl;
  } else if (value.IsNum()) {
    cout << value.GetNum() << endl;
  } else {
    cout << value.GetStr() << endl;
  }
}

void Interpreter::Visit(VarStmt &stmt) {
  global_env.Define(stmt.token->lexeme, Evaluate(*(stmt.expr)));
}

LoxValue Interpreter::Evaluate(Expr &expr) { return expr.Accept(*this); }

void Interpreter::Execute() {
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