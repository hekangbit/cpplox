#include "interpreter.h"


LoxValue Interpreter::Visit(NumberLiteralExpr &expr) {
  return LoxValue("");
}

LoxValue Interpreter::Visit(StringLiteralExpr &expr) {
  return LoxValue("");
}

LoxValue Interpreter::Visit(UnaryExpr &expr) {
  return LoxValue("");
}

LoxValue Interpreter::Visit(BinaryExpr &expr) {
  return LoxValue("");
}

LoxValue Interpreter::Visit(GroupingExpr &expr) {
  return LoxValue("");
}

LoxValue Interpreter::Visit(NullLiteralExpr &expr) {
  return LoxValue("");
}

LoxValue Interpreter::Visit(BoolLiteralExpr &expr) {
  return LoxValue("");
}

void Interpreter::Visit(ExprStmt &stmt) {

}

void Interpreter::Visit(PrintStmt &stmt) {

}

void Interpreter::Visit(VarStmt &stmt) {

}

LoxValue Interpreter::Evaluate(Expr &expr) {
  return expr.Accept(*this);
}

void Interpreter::Execute() {
  for (auto &statement : statements) {
    
  }
}