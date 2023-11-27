#ifndef TOKEN_H
#define TOKEN_H

#include "common.h"

typedef enum {
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  STAR,
  BANG,
  BANG_EQUAL,
  EQUAL,
  ASSIGN,
  LESS_EQUAL,
  LESS,
  GREATER_EQUAL,
  GREATER,
  SLASH,
  IDENTIFIER,
  STRING,
  NUMBER,
  AND,
  OR,
  PRINT,
  RETURN,
  SUPER,
  _THIS,
  TRUE,
  FALSE,
  VAR,
  CLASS,
  FUN,
  IF,
  _ELSE,
  FOR,
  WHILE,
  NIL,
  TYPE_NUM
} TokenType;

// class Literal {
// public:
//   Literal(string str) : is_num(false), this.str(str) {};
//   Literal(int32_t num) : is_num(true), this.num(num) {};
// private:
//   string str;
//   int32_t num;
//   bool is_num;
// };

class Token {
public:
  Token(TokenType type, string lexeme, int32_t line)
      : type(type), lexeme(lexeme), line(line){};
  Token(TokenType type, string lexeme, int32_t literalNum, int32_t line)
      : type(type), lexeme(lexeme), literalNum(literalNum), line(line){};
  Token(TokenType type, string lexeme, string literalStr, int32_t line)
      : type(type), lexeme(lexeme), literalStr(literalStr), line(line){};
  string toString();
  string GetText() const { return lexeme + ", line:" + to_string(line); };

private:
  TokenType type;
  int32_t literalNum;
  string literalStr;
  string lexeme;
  int32_t line;
};

#endif