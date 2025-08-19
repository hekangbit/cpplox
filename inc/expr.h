#ifndef EXPR_H
#define EXPR_H

#include "common.h"
#include "token.h"
#include "value.h"
#include "visitor.h"

class Expr {
public:
  Expr() {}
  virtual ~Expr() {}
  virtual Value Accept(Visitor &visitor) = 0;
};

using expr_t = shared_ptr<Expr>;

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
  UnaryExpr(token_t op, expr_t right) : op(op), right(right) {}
  Value Accept(Visitor &visitor) { return visitor.Visit(*this); }
  expr_t right;
  token_t op;
};

class BinaryExpr : public Expr {
public:
  BinaryExpr(expr_t left, token_t op, expr_t right)
      : left(left), op(op), right(right) {}
  Value Accept(Visitor &visitor) { return visitor.Visit(*this); }
  expr_t left;
  expr_t right;
  token_t op;
};

class GroupingExpr : public Expr {
public:
  GroupingExpr(expr_t expr) : expr(expr) {}
  Value Accept(Visitor &visitor) { return visitor.Visit(*this); }
  expr_t expr;
};

class VariableExpr : public Expr {
public:
  VariableExpr(token_t token) : token(token) {}
  Value Accept(Visitor &visitor) { return visitor.Visit(*this); }
  token_t token;
};

class AssignExpr : public Expr {
public:
  AssignExpr(token_t name, expr_t value) : name(name), value(value) {}
  Value Accept(Visitor &visitor) { return visitor.Visit(*this); }
  expr_t value;
  token_t name;
};

#endif