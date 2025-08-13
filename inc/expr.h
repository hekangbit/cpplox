#ifndef EXPR_H
#define EXPR_H

#include "common.h"
#include "token.h"
#include "value.h"
#include "visitor.h"

class Expr {
public:
  Expr() {}
  virtual Value Accept(Visitor &visitor) = 0;
};

class NullLiteralExpr : public Expr {
public:
  NullLiteralExpr() {}
  Value Accept(Visitor &visitor) { return visitor.Visit(*this); }
};

class BoolLiteralExpr : public Expr {
public:
  BoolLiteralExpr(bool val) : val(val) {}
  Value Accept(Visitor &visitor) { return visitor.Visit(*this); }
  bool val;
};

class NumberLiteralExpr : public Expr {
public:
  NumberLiteralExpr(double num) : num(num) {}
  Value Accept(Visitor &visitor) { return visitor.Visit(*this); }
  double num;
};

class StringLiteralExpr : public Expr {
public:
  StringLiteralExpr(string &s) : str(s) {}
  Value Accept(Visitor &visitor) { return visitor.Visit(*this); }
  string str;
};

class UnaryExpr : public Expr {
public:
  UnaryExpr(Token *op, Expr *right) : op(*op), right(right) {}
  Value Accept(Visitor &visitor) { return visitor.Visit(*this); }
  Expr *right;
  Token op;
};

class BinaryExpr : public Expr {
public:
  BinaryExpr(Expr *left, Token *op, Expr *right)
      : left(left), op(*op), right(right) {}
  Value Accept(Visitor &visitor) { return visitor.Visit(*this); }
  Expr *left;
  Expr *right;
  Token op;
};

class GroupingExpr : public Expr {
public:
  GroupingExpr(Expr *expr) : expr(expr) {}
  Value Accept(Visitor &visitor) { return visitor.Visit(*this); }
  Expr *expr;
};

class VariableExpr : public Expr {
public:
  VariableExpr(Token *var) : var(*var) {}
  Value Accept(Visitor &visitor) { return visitor.Visit(*this); }
  Token var;
};

class AssignExpr : public Expr {
public:
  AssignExpr(Token name, Expr *value) : name(name), value(value) {}
  Value Accept(Visitor &visitor) { return visitor.Visit(*this); }
  Expr *value;
  Token name;
};

#endif