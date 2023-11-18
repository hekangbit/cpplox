#ifndef EXPR_H
#define EXPR_H

#include "common.h";

class Expr {
public:
  Expr();

private:
};

class LiteralEXpr : Expr {
public:
  LiteralEXpr(string s);
  LiteralEXpr(long num);

private:
};

class BinaryExpr : Expr {
public:
  BinaryExpr();
};

#endif