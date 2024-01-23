#include "parser.h"
#include "astprinter.h"
#include "expr.h"

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

Expr *Parser::Expression() { return Equality(); }

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
  throw Error(Peek(), "expect expr.");
  return nullptr;
}

ParserException Parser::Error(Token *token, string message) {
  if (token == nullptr) {
    message = "at the end, " + message;
    int32_t last_line_num = 0;
    if (tokens.size()) {
      last_line_num = tokens.back().line;
    }
    error(last_line_num, message);
  } else {
    message = "found token " + token->lexeme + ", " + message;
    error(token->line, message);
  }
  return ParserException();
}

Expr *Parser::Parse() {
  try {
    return Expression();
  } catch (const ParserException &e) {
    cout << "Catch Parser exception" << endl;
    cout << e.what() << endl;
    return nullptr;
  }
  return nullptr;
}
