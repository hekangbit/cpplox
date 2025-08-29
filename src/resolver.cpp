#include "resolver.h"

void Resolver::Declare(token_t token) {
  if (scopes.empty()) {
    return;
  }
  auto &scope = scopes.back();
  if (scope.count(token->lexeme)) {
    LoxError(token, "Double define in this scope.");
  }
  scope[token->lexeme] = false;
}

void Resolver::Define(token_t token) {
  if (scopes.empty()) {
    return;
  }
  auto &scope = scopes.back();
  scope[token->lexeme] = true;
}

void Resolver::BeginScope() {
  scopes.emplace_back();
}

void Resolver::EndScope() {
  scopes.pop_back();
}

void Resolver::Resolve(expr_t expr) {
  if (expr) {
    expr->Accept(*this);
  }
}

void Resolver::Resolve(stmt_t statement) {
  if (statement) {
    statement->Accept(*this);
  }
}

void Resolver::Resolve(vector<stmt_t> &statements) {
  for (auto stmt : statements) {
    Resolve(stmt);
  }
}

void Resolver::ResolveLocal(Expr *expr, token_t token) {
  for (int i = scopes.size() - 1; i >= 0; i--) {
    if (scopes[i].count(token->lexeme)) {
      interpreter->Resolve(expr, scopes.size() - 1 - i);
    }
  }
}

void Resolver::ResolveFunction(FunctionStmt &func) {
  bool prev_state = is_func_enclosing;
  is_func_enclosing = true;
  BeginScope();
  for (auto para : func.params) {
    Declare(para);
    Define(para);
  }
  Resolve(func.body);
  EndScope();
  is_func_enclosing = prev_state;
}

Value Resolver::Visit(NumberLiteralExpr &expr) {
  return Value();
}

Value Resolver::Visit(StringLiteralExpr &expr) {
  return Value();
}

Value Resolver::Visit(UnaryExpr &expr) {
  Resolve(expr.right);
  return Value();
}

Value Resolver::Visit(BinaryExpr &expr) {
  Resolve(expr.left);
  Resolve(expr.right);
  return Value();
}

Value Resolver::Visit(GroupingExpr &expr) {
  Resolve(expr.expr);
  return Value();
}

Value Resolver::Visit(NullLiteralExpr &expr) {
  return Value();
}

Value Resolver::Visit(BoolLiteralExpr &expr) {
  return Value();
}

Value Resolver::Visit(LogicalExpr &expr) {
  Resolve(expr.left);
  Resolve(expr.right);
  return Value();
}

Value Resolver::Visit(VariableExpr &expr) {
  if (scopes.empty()) {
    return Value();
  }

  auto it = scopes.back().find(expr.token->lexeme);
  if (it != scopes.back().end() && it->second == false) {
    LoxError(expr.token, "Can't read local variable in its own initializer.");
  }
  ResolveLocal(&expr, expr.token);
  return Value();
}

Value Resolver::Visit(AssignExpr &expr) {
  Resolve(expr.value); // right
  ResolveLocal(&expr, expr.name); // left
  return Value();
}

Value Resolver::Visit(CallExpr &expr) {
  Resolve(expr.callee);
  for (auto arg : expr.arguments) {
    Resolve(arg);
  }
  return Value();
}

void Resolver::Visit(ExprStmt &stmt) {
  Resolve(stmt.expr);
}

void Resolver::Visit(PrintStmt &stmt) {
  Resolve(stmt.expr);
}

void Resolver::Visit(BlockStmt &stmt) {
  BeginScope();
  Resolve(stmt.statements);
  EndScope();
}

void Resolver::Visit(VarStmt &stmt) {
  Declare(stmt.token);
  Resolve(stmt.initializer);
  Define(stmt.token);
}

void Resolver::Visit(IfStmt &stmt) {
  Resolve(stmt.condition);
  Resolve(stmt.thenStmt);
  Resolve(stmt.elseStmt);
}

void Resolver::Visit(WhileStmt &stmt) {
  Resolve(stmt.condition);
  Resolve(stmt.body);
}

void Resolver::Visit(BreakStmt &stmt) {
}

void Resolver::Visit(FunctionStmt &stmt) {
  Declare(stmt.name);
  Define(stmt.name);
  ResolveFunction(stmt);
}

void Resolver::Visit(ReturnStmt &stmt) {
  if (!is_func_enclosing) {
    LoxError(stmt.token, "Can't return from top-level code.");
  }
  Resolve(stmt.expr);
}