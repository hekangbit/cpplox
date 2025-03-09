#include "astprinter.h"

void AstPrinter::Walk(Expr &expr) {
  LoxValue res = expr.Accept(*this);
  cout << res.GetStr() << endl;
}

string AstPrinter::Parenthesize(string &name, vector<Expr *> &exprs) {
  string res;
  // cout << "(" << name;
  res += "(";
  for (auto expr : exprs) {
    // cout << " ";
    // res += expr->Accept(*this);
    LoxValue tmp = expr->Accept(*this);
    res += tmp.GetStr();
  }
  // cout << ")";
  res += ")";
  return res;
}

LoxValue AstPrinter::Visit(NumberLiteralExpr &expr) {
  // cout << expr.num;
  return to_string(expr.num);
}

LoxValue AstPrinter::Visit(StringLiteralExpr &expr) {
  // cout << expr.str;
  return expr.str;
}

LoxValue AstPrinter::Visit(NullLiteralExpr &expr) {
  // cout << "null";
  string res("nil");
  return res;
}

LoxValue AstPrinter::Visit(BoolLiteralExpr &expr) {
  string res = expr.val ? "true" : "false";
  // cout << res;
  return res;
}

LoxValue AstPrinter::Visit(UnaryExpr &expr) {
  vector<Expr *> exprs{expr.right};
  return Parenthesize(expr.op.lexeme, exprs);
}

LoxValue AstPrinter::Visit(BinaryExpr &expr) {
  vector<Expr *> exprs{expr.left, expr.right};
  return Parenthesize(expr.op.lexeme, exprs);
}

LoxValue AstPrinter::Visit(GroupingExpr &expr) {
  string res;
  res += "(";
  // cout << "(";
  LoxValue tmp = expr.expr->Accept(*this);
  res += tmp.GetStr();
  // cout << ")";
  res += ")";
  return res;
}

LoxValue AstPrinter::Visit(VariableExpr &expr) { return expr.var.lexeme; }

LoxValue AstPrinter::Visit(AssignExpr &expr) {
  return LoxValue();
}

void AstPrinter::Visit(ExprStmt &stmt) {}

void AstPrinter::Visit(PrintStmt &stmt) {}

void AstPrinter::Visit(BlockStmt &stmt) {}

void AstPrinter::Visit(VarStmt &stmt) {}