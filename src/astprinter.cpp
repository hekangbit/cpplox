#include "astprinter.h"

void AstPrinter::Walk(Expr &expr) { expr.Accept(*this); }

void AstPrinter::Visit(NumberLiteralExpr &expr) {}

void AstPrinter::Visit(StringLiteralExpr &expr) {}

void AstPrinter::Visit(UnaryExpr &expr) {}

void AstPrinter::Visit(BinaryExpr &expr) {}

void AstPrinter::Visit(GroupingExpr &expr) {}
