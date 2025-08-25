#ifndef PARSER_H
#define PARSER_H

#include "common.h"
#include "error.h"
#include "expr.h"
#include "stmt.h"
#include "token.h"
#include <exception>
#include <vector>
#include <list>

class ParserException : public exception {
public:
  const char *what() const throw() { return "ParserException"; }
};

class Parser {
public:
  Parser(vector<Token> tokens) : tokens(tokens) {}

  bool IsAtEnd();
  bool Check(TokenType type);
  bool Match(initializer_list<TokenType> token_types);
  token_t Peek();
  token_t Previous();
  token_t Advance();
  token_t Consume(TokenType type, string message);

  ParserException Error(token_t token, string message);
  void Synchronize();

  expr_t FinishCall(expr_t expr);

  expr_t Assignment();
  expr_t LogicalOr();
  expr_t LogicalAnd();
  expr_t Equality();
  expr_t Comparison();
  expr_t Term();
  expr_t Factor();
  expr_t Unary();
  expr_t Call();
  expr_t Primary();
  expr_t Expression();

  stmt_t printStatement();
  stmt_t ifStatement();
  stmt_t blockStatement();
  stmt_t whileStatement();
  stmt_t forStatement();
  stmt_t breakStatement();
  stmt_t expressionStatemenmt();
  stmt_t Statement();
  stmt_t VarDeclaration();
  stmt_t FuncDeclaration();
  stmt_t Declaration();
  vector<stmt_t> Parse();

private:
  vector<Token> tokens;
  int32_t current = 0;
  bool isInsideLoop = false;
};

#endif