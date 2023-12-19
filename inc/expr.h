#ifndef EXPR_H
#define EXPR_H

#include "common.h"
#include "token.h"

class Expr {
public:
  Expr() {};
};

class NumberLiteralExpr : public Expr {
public:
  NumberLiteralExpr(double num) : num(num) {};
private:
  double num;
};

class StringLiteralExpr : public Expr {
public:
  StringLiteralExpr(string &s) : str(s) {};
private:
  string str;
};

class UnaryExpr : public Expr {
public:
  UnaryExpr(Token *op, Expr *right) : op(*op), right(*right) {};
private:
  Expr right;
  Token op;
};

class BinaryExpr : public Expr {
public:
  BinaryExpr(Expr *left, Token *op, Expr *right) : left(*left), op(*op), right(*right) {};
private:
  Expr left;
  Expr right;
  Token op;
};

class GroupingExpr : public Expr {
public:
  GroupingExpr(Expr *expr) : expr(*expr) {};
private:
  Expr expr;
};


#endif