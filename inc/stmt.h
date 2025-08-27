#ifndef STMT_H
#define STMT_H

#include "common.h"
#include "expr.h"
#include "token.h"
#include "visitor.h"

class Stmt {
public:
  Stmt() {}
  virtual ~Stmt() {}
  virtual void Accept(Visitor &visitor) = 0;
};

using stmt_t = shared_ptr<Stmt>;

class ExprStmt : public Stmt {
public:
  ExprStmt(expr_t expr) : expr(expr) {}
  virtual void Accept(Visitor &visitor) {
    visitor.Visit(*this);
  }
  expr_t expr;
};

class PrintStmt : public Stmt {
public:
  PrintStmt(token_t token, expr_t expr) : token(token), expr(expr) {}
  virtual void Accept(Visitor &visitor) {
    visitor.Visit(*this);
  }
  token_t token;
  expr_t expr;
};

class VarStmt : public Stmt {
public:
  VarStmt(token_t token, expr_t initializer)
      : token(token), initializer(initializer) {}
  virtual void Accept(Visitor &visitor) {
    visitor.Visit(*this);
  }
  token_t token;
  expr_t initializer;
};

class BlockStmt : public Stmt {
public:
  BlockStmt(vector<stmt_t> statements) : statements(statements) {}
  virtual void Accept(Visitor &visitor) {
    visitor.Visit(*this);
  };
  vector<stmt_t> statements;
};

class IfStmt : public Stmt {
public:
  IfStmt(expr_t condition, stmt_t thenStmt, stmt_t elseStmt)
      : condition(condition), thenStmt(thenStmt), elseStmt(elseStmt) {}
  virtual void Accept(Visitor &visitor) {
    visitor.Visit(*this);
  };
  expr_t condition;
  stmt_t thenStmt;
  stmt_t elseStmt;
};

class WhileStmt : public Stmt {
public:
  WhileStmt(expr_t condition, stmt_t body) : condition(condition), body(body) {}
  virtual void Accept(Visitor &visitor) {
    visitor.Visit(*this);
  }
  expr_t condition;
  stmt_t body;
};

class BreakStmt : public Stmt {
public:
  BreakStmt() {}
  virtual void Accept(Visitor &visitor) {
    visitor.Visit(*this);
  }
};

class FunctionStmt : public Stmt {
public:
  FunctionStmt(token_t name, vector<token_t> params, stmt_t body)
      : name(name), params(params), body(body) {}
  virtual void Accept(Visitor &visitor) {
    visitor.Visit(*this);
  }
  token_t name;
  vector<token_t> params;
  stmt_t body;
};

class ReturnStmt : public Stmt {
public:
  ReturnStmt(token_t token, expr_t expr) : token(token), expr(expr) {}
  virtual void Accept(Visitor &visitor) {
    visitor.Visit(*this);
  }
  token_t token;
  expr_t expr;
};

#endif