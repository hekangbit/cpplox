#ifndef EXPR_H
#define EXPR_H

#include "common.h"
#include "token.h"

class NumberLiteralExpr;
class StringLiteralExpr;
class UnaryExpr;
class BinaryExpr;
class GroupingExpr;
class NullLiteralExpr;
class BoolLiteralExpr;

class Visitor {
public:
  virtual void Visit(NumberLiteralExpr &expr) = 0;
  virtual void Visit(StringLiteralExpr &expr) = 0;
  virtual void Visit(UnaryExpr &expr) = 0;
  virtual void Visit(BinaryExpr &expr) = 0;
  virtual void Visit(GroupingExpr &expr) = 0;
  virtual void Visit(NullLiteralExpr &expr) = 0;
  virtual void Visit(BoolLiteralExpr &expr) = 0;
};

class Expr {
public:
  Expr(){};
  virtual void Accept(Visitor &visitor) {}
};

class NullLiteralExpr : public Expr {
public:
  NullLiteralExpr() {}
  void Accept(Visitor &visitor) { visitor.Visit(*this); }
};

class BoolLiteralExpr : public Expr {
public:
  BoolLiteralExpr(bool val) : val(val) {}
  void Accept(Visitor &visitor) { visitor.Visit(*this); }
  bool val;
};

class NumberLiteralExpr : public Expr {
public:
  NumberLiteralExpr(double num) : num(num) {}
  void Accept(Visitor &visitor) { visitor.Visit(*this); }
  double num;
};

class StringLiteralExpr : public Expr {
public:
  StringLiteralExpr(string &s) : str(s) {}
  void Accept(Visitor &visitor) { visitor.Visit(*this); }
  string str;
};

class UnaryExpr : public Expr {
public:
  UnaryExpr(Token *op, Expr *right) : op(*op), right(right) {}
  void Accept(Visitor &visitor) { visitor.Visit(*this); }
  Expr *right;
  Token op;
};

class BinaryExpr : public Expr {
public:
  BinaryExpr(Expr *left, Token *op, Expr *right)
      : left(left), op(*op), right(right) {}
  void Accept(Visitor &visitor) { visitor.Visit(*this); }
  Expr *left;
  Expr *right;
  Token op;
};

class GroupingExpr : public Expr {
public:
  GroupingExpr(Expr *expr) : expr(expr) {}
  void Accept(Visitor &visitor) { visitor.Visit(*this); }
  Expr *expr;
};

#endif