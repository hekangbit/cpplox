#ifndef PARSER_H
#define PARSER_H
#include "common.h"
#include "error.h"
#include "expr.h"
#include "token.h"
#include <exception>
#include <vector>

class ParserException : public exception {
public:
  const char *what() const throw() { return "ParserException"; }
};

class Parser {
public:
  Parser(vector<Token> tokens) : tokens(tokens){};

  Expr *Parse();
  Expr *Expression();
  Expr *Equality();
  Expr *Comparison();
  Expr *Term();
  Expr *Factor();
  Expr *Unary();
  Expr *Primary();

  bool IsAtEnd();
  Token *Peek();
  Token *Previous();
  Token *Advance();
  Token *Consume(TokenType type, string message);
  bool Check(TokenType type);
  bool Match(initializer_list<TokenType> token_types);
  ParserException Error(Token *token, string message);

private:
  vector<Token> tokens;
  int32_t current = 0;
};

#endif