#include "parser.h"

bool Parser::IsAtEnd() { return current >= tokens.size(); }

Token *Parser::Previous() {
  if (current == 0) {
    return nullptr;
  }
  return &(tokens[current - 1]);
}

Token *Parser::Advance() {
  if (IsAtEnd()) {
    return Previous();
  }
  current++;
  return Previous();
}

Token *Parser::Consume(TokenType type, string message) {
  if (Check(type)) {
    return Advance();
  }
  throw Error(Peek(), message);
}

bool Parser::Check(TokenType type) {
  if (IsAtEnd()) {
    return false;
  }
  return Peek()->type == type;
}

bool Parser::Match(initializer_list<TokenType> token_types) {
  for (auto &it : token_types) {
    if (Check(it)) {
      Advance();
      return true;
    }
  }
  return false;
}

Token *Parser::Peek() {
  if (IsAtEnd()) {
    return nullptr;
  }
  return &(tokens[current]);
}

ParserException Parser::Error(Token *token, string message) {
  if (token == nullptr) {
    message = "at the end, " + message;
    error(tokens.size() ? tokens.back().line : 0, message);
  } else {
    message = "found token " + token->lexeme + ", " + message;
    error(token->line, message);
  }
  return ParserException();
}

void Parser::Synchronize() {}

Expr *Parser::Equality() {
  Expr *expr = Comparison();
  while (Match({EQUAL_EQUAL, BANG_EQUAL})) {
    Token *op = Previous();
    Expr *right = Comparison();
    expr = new BinaryExpr(expr, op, right);
  }
  return expr;
}

Expr *Parser::Comparison() {
  Expr *expr = Term();
  while (Match({LESS_EQUAL, LESS, GREATER_EQUAL, GREATER})) {
    Token *op = Previous();
    Expr *right = Term();
    expr = new BinaryExpr(expr, op, right);
  }
  return expr;
}

Expr *Parser::Term() {
  Expr *expr = Factor();
  while (Match({PLUS, MINUS})) {
    Token *op = Previous();
    Expr *right = Factor();
    expr = new BinaryExpr(expr, op, right);
  }
  return expr;
}

Expr *Parser::Factor() {
  Expr *expr = Unary();
  while (Match({STAR, SLASH})) {
    Token *op = Previous();
    Expr *right = Unary();
    expr = new BinaryExpr(expr, op, right);
  }
  return expr;
}

Expr *Parser::Unary() {
  if (Match({MINUS, BANG})) {
    Token *op = Previous();
    Expr *expr = Unary();
    return new UnaryExpr(op, expr);
  }
  return Primary();
}

Expr *Parser::Primary() {
  if (Match({TRUE})) {
    return new BoolLiteralExpr(true);
  }
  if (Match({FALSE})) {
    return new BoolLiteralExpr(false);
  }
  if (Match({NIL})) {
    return new NullLiteralExpr();
  }
  if (Match({NUMBER})) {
    return new NumberLiteralExpr(Previous()->literalNum);
  }
  if (Match({STRING})) {
    return new StringLiteralExpr(Previous()->literalStr);
  }
  if (Match({LEFT_PAREN})) {
    Expr *expr = Expression();
    Consume(RIGHT_PAREN, "expect ')' after expression.");
    return new GroupingExpr(expr);
  }
  if (Match({IDENTIFIER})) {
    return new VariableExpr(Previous());
  }
  throw Error(Peek(), "expect expr.");
  return nullptr;
}

Expr *Parser::Expression() { return Equality(); }

Stmt *Parser::VarDeclaration() {
  Token *token = Consume(IDENTIFIER, "Expect a variable name");
  Expr *initializer = nullptr;
  if (Match({EQUAL})) {
    initializer = Expression();
  }
  Consume(SEMICOLON, "Expect ';' in var declaration statement");
  return new VarStmt(token, initializer);
}

Stmt *Parser::Declaration() {
  Stmt *result;
  try {
    if (Match({VAR})) {
      result = VarDeclaration();
    } else if (Match({PRINT})) {
      result = new PrintStmt(Peek(), Expression());
      Consume(SEMICOLON, "Expect ';' in print statement");
    } else {
      result = new ExprStmt(Expression());
      Consume(SEMICOLON, "Expect ';' in expr statement");
    }
    return result;

  } catch (const ParserException &e) {
    cout << "Catch Parser exception" << endl;
    cerr << e.what() << endl;
    Synchronize();
    return nullptr;
  }
}

vector<Stmt *> Parser::Parse() {
  vector<Stmt *> statements;
  while (!IsAtEnd()) {
    statements.push_back(Declaration());
  }
  return statements;
}