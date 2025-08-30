#include "astprinter.h"

void AstPrinter::Walk(Expr &expr) {
  Value res = expr.Accept(*this);
  cout << res << endl;
}

string AstPrinter::Parenthesize(string &name, vector<expr_t> &exprs) {
  string res;
  // cout << "(" << name;
  res += "(";
  for (auto expr : exprs) {
    // cout << " ";
    // res += expr->Accept(*this);
    Value tmp = expr->Accept(*this);
    res += tmp.toString();
  }
  // cout << ")";
  res += ")";
  return res;
}

Value AstPrinter::Visit(NumberLiteralExpr &expr) {
  // cout << expr.num;
  return to_string(expr.num);
}

Value AstPrinter::Visit(StringLiteralExpr &expr) {
  // cout << expr.str;
  return expr.str;
}

Value AstPrinter::Visit(NullLiteralExpr &expr) {
  // cout << "null";
  string res("nil");
  return res;
}

Value AstPrinter::Visit(BoolLiteralExpr &expr) {
  string res = expr.val ? "true" : "false";
  // cout << res;
  return res;
}

Value AstPrinter::Visit(UnaryExpr &expr) {
  vector<expr_t> exprs{expr.right};
  return Parenthesize(expr.op->lexeme, exprs);
}

Value AstPrinter::Visit(BinaryExpr &expr) {
  vector<expr_t> exprs{expr.left, expr.right};
  return Parenthesize(expr.op->lexeme, exprs);
}

Value AstPrinter::Visit(GroupingExpr &expr) {
  string res;
  res += "(";
  // cout << "(";
  Value tmp = expr.expr->Accept(*this);
  res += tmp.toString();
  // cout << ")";
  res += ")";
  return res;
}

Value AstPrinter::Visit(VariableExpr &expr) {
  return expr.token->lexeme;
}

Value AstPrinter::Visit(LogicalExpr &expr) {
  vector<expr_t> exprs{expr.left, expr.right};
  return Parenthesize(expr.op->lexeme, exprs);
}

Value AstPrinter::Visit(AssignExpr &expr) {
  return Value();
}

Value AstPrinter::Visit(CallExpr &expr) {
  return string("()");
}

void AstPrinter::Visit(ExprStmt &stmt) {}

void AstPrinter::Visit(PrintStmt &stmt) {}

void AstPrinter::Visit(BlockStmt &stmt) {}

void AstPrinter::Visit(VarStmt &stmt) {}

void AstPrinter::Visit(IfStmt &stmt) {}

void AstPrinter::Visit(WhileStmt &stmt) {}

void AstPrinter::Visit(BreakStmt &stmt) {}

void AstPrinter::Visit(FunctionStmt &stmt) {}

void AstPrinter::Visit(ReturnStmt &stmt) {}

void AstPrinter::Visit(ClassStmt &stmt) {}
