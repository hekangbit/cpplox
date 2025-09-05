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

void Resolver::ResolveFunction(FunctionStmt &func, FunctionType type) {
  FunctionType prev_state = func_type;
  func_type = type;
  BeginScope();
  for (auto para : func.params) {
    Declare(para);
    Define(para);
  }
  Resolve(func.body);
  EndScope();
  func_type = prev_state;
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
  Resolve(expr.value);            // right
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

Value Resolver::Visit(GetExpr &expr) {
  Resolve(expr.object);
  return Value();
}

Value Resolver::Visit(SetExpr &expr) {
  Resolve(expr.object);
  Resolve(expr.value);
  return Value();
}

Value Resolver::Visit(ThisExpr &expr) {
  if (class_type == CLASS_TYPE_NONE) {
    LoxError(expr.keyword, "Can't use 'this' outside of a class.");
  }
  ResolveLocal(&expr, expr.keyword);
  return Value();
}

Value Resolver::Visit(SuperExpr &expr) {
  if (class_type == CLASS_TYPE_NONE) {
    LoxError(expr.keyword, "Can't use 'super' outside of a class.");
  } else if (class_type != CLASS_TYPE_SUBCLASS) {
    LoxError(expr.keyword, "Can't use 'super' in a class without superclass.");
  }
  ResolveLocal(&expr, expr.keyword);
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

void Resolver::Visit(BreakStmt &stmt) {}

void Resolver::Visit(FunctionStmt &stmt) {
  Declare(stmt.name);
  Define(stmt.name);
  ResolveFunction(stmt, FUNC_TYPE_FUNCTION);
}

void Resolver::Visit(ReturnStmt &stmt) {
  if (func_type == FUNC_TYPE_NONE) {
    LoxError(stmt.token, "Can't return from top-level code.");
  }
  if (stmt.expr == nullptr) {
    return;
  }
  if (func_type == FUNC_TYPE_INIT) {
    LoxError(stmt.token, "Can't return a value from lox initializer.");
  }
  Resolve(stmt.expr);
}

void Resolver::Visit(ClassStmt &stmt) {
  ClassType prev = class_type;
  class_type = CLASS_TYPE_CLASS;
  Declare(stmt.name);
  Define(stmt.name);
  if (stmt.superclass &&
      (stmt.superclass->token->lexeme.compare(stmt.name->lexeme) == 0)) {
    LoxError(stmt.superclass->token, "A class can't inherit from itself.");
  }
  if (stmt.superclass) {
    class_type = CLASS_TYPE_SUBCLASS;
    Resolve(stmt.superclass);
    BeginScope();
    scopes.back()["super"] = true;
  }
  BeginScope();
  // define "this" in an implicit scope just outside of the block for the method
  // body.
  scopes.back()["this"] = true;
  for (auto method : stmt.methods) {
    FunctionType type = FUNC_TYPE_METHOD;
    if (method->name->lexeme.compare("init") == 0) {
      type = FUNC_TYPE_INIT;
    }
    ResolveFunction(*(method.get()), type);
  }
  EndScope();
  if (stmt.superclass) {
    EndScope();
  }
  class_type = prev;
}