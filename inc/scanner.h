#ifndef SCANNER_H
#define SCANNER_H

#include "common.h"
#include "token.h"
#include <vector>

class Scanner {
public:
  Scanner(string str) : source(str) {};
  void ScanToken();
  void ScanTokens();
  vector<Token> &GetTokens();

private:
  bool IsAtEnd() const;
  char Advance();

  void AddToken(TokenType type);
  void AddToken(TokenType type, int32_t num);
  void AddToken(TokenType type, string str);

  string source;
  int32_t start = 0;
  int32_t current = 0;
  int32_t line = 0;
  vector<Token> tokens;
};

#endif