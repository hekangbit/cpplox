#ifndef STMT_H
#define STMT_H

#include "common.h"
#include "expr.h"
#include "token.h"
#include "visitor.h"
#include <list>

class Stmt {
public:
  Stmt() {}
  virtual void Accept(Visitor &visitor) = 0;
};

class ExprStmt : public Stmt {
public:
  ExprStmt(Expr *expr) : expr(expr) {}
  virtual void Accept(Visitor &visitor) { visitor.Visit(*this); }
  Expr *expr;
};

class PrintStmt : public Stmt {
public:
  PrintStmt(Token *token, Expr *expr) : token(token), expr(expr) {}
  virtual void Accept(Visitor &visitor) { visitor.Visit(*this); }
  Token *token;
  Expr *expr;
};

class VarStmt : public Stmt {
public:
  VarStmt(Token *token, Expr *initializer) : token(token), initializer(initializer) {}
  virtual void Accept(Visitor &visitor) { visitor.Visit(*this); }
  Token *token;
  Expr *initializer;
};

class BlockStmt : public Stmt {
public:
  BlockStmt(list<Stmt*> statements) : statements(statements) {}
  virtual void Accept(Visitor &visitor) { visitor.Visit(*this); };
  list<Stmt*> statements;
};

#endif