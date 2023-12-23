#ifndef EXPR_H
#define EXPR_H

#include "common.h"
#include "token.h"

class NumberLiteralExpr;
class StringLiteralExpr;
class UnaryExpr;
class BinaryExpr;
class GroupingExpr;

class Visitor {
public:
  virtual void Visit(NumberLiteralExpr &expr) = 0;
  virtual void Visit(StringLiteralExpr &expr) = 0;
  virtual void Visit(UnaryExpr &expr) = 0;
  virtual void Visit(BinaryExpr &expr) = 0;
  virtual void Visit(GroupingExpr &expr) = 0;
};

class Expr {
public:
  Expr(){};
  virtual void Accept(Visitor &visitor){};
};

class NumberLiteralExpr : public Expr {
public:
  NumberLiteralExpr(double num) : num(num){};
  void Accept(Visitor &visitor) { visitor.Visit(*this); };

private:
  double num;
};

class StringLiteralExpr : public Expr {
public:
  StringLiteralExpr(string &s) : str(s){};
  void Accept(Visitor &visitor) { visitor.Visit(*this); };

private:
  string str;
};

class UnaryExpr : public Expr {
public:
  UnaryExpr(Token *op, Expr *right) : op(*op), right(*right){};
  void Accept(Visitor &visitor) { visitor.Visit(*this); };

private:
  Expr right;
  Token op;
};

class BinaryExpr : public Expr {
public:
  BinaryExpr(Expr *left, Token *op, Expr *right)
      : left(*left), op(*op), right(*right){};
  void Accept(Visitor &visitor) { visitor.Visit(*this); };

private:
  Expr left;
  Expr right;
  Token op;
};

class GroupingExpr : public Expr {
public:
  GroupingExpr(Expr *expr) : expr(*expr){};
  void Accept(Visitor &visitor) { visitor.Visit(*this); };

private:
  Expr expr;
};

#endif