#include "astprinter.h"

void AstPrinter::Walk(Expr &expr) {
  expr.Accept(*this);
  cout << endl;
}

void AstPrinter::Visit(NumberLiteralExpr &expr) { cout << expr.num; }

void AstPrinter::Visit(StringLiteralExpr &expr) {}

void AstPrinter::Visit(UnaryExpr &expr) {}

void AstPrinter::Visit(BinaryExpr &expr) {
  vector<Expr *> exprs{expr.left, expr.right};
  Parenthesize(expr.op.lexeme, exprs);
}

void AstPrinter::Visit(GroupingExpr &expr) {}

void AstPrinter::Parenthesize(string &name, vector<Expr *> &exprs) {
  cout << "(" << name;
  for (auto expr : exprs) {
    cout << " ";
    expr->Accept(*this);
  }
  cout << ")";
}