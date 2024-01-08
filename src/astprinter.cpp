#include "astprinter.h"

void AstPrinter::Walk(Expr &expr) {
  expr.Accept(*this);
  cout << endl;
}

void AstPrinter::Visit(NumberLiteralExpr &expr) { cout << expr.num; }

void AstPrinter::Visit(StringLiteralExpr &expr) { cout << expr.str; }

void AstPrinter::Visit(NullLiteralExpr &expr) { cout << "null"; }

void AstPrinter::Visit(BoolLiteralExpr &expr) {
  string res = expr.val ? "true" : "false";
  cout << res;
}

void AstPrinter::Visit(UnaryExpr &expr) {
  vector<Expr *> exprs{expr.right};
  Parenthesize(expr.op.lexeme, exprs);
}

void AstPrinter::Visit(BinaryExpr &expr) {
  vector<Expr *> exprs{expr.left, expr.right};
  Parenthesize(expr.op.lexeme, exprs);
}

void AstPrinter::Visit(GroupingExpr &expr) {
  cout << "(";
  expr.expr->Accept(*this);
  cout << ")";
}

void AstPrinter::Parenthesize(string &name, vector<Expr *> &exprs) {
  cout << "(" << name;
  for (auto expr : exprs) {
    cout << " ";
    expr->Accept(*this);
  }
  cout << ")";
}