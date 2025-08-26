#ifndef SCANNER_H
#define SCANNER_H

#include "common.h"
#include "token.h"
#include <map>
#include <vector>

class Scanner {
public:
  Scanner(string str) : source(str) {}
  void ScanToken();
  void ScanTokens();
  void Dump();
  vector<Token> GetTokens() {
    return tokens;
  }

private:
  bool IsAtEnd() const;
  bool IsDigit(char c) const;
  bool IsAlpha(char c) const;
  bool IsAlphaNumeric(char c) const;
  bool Match(char c);
  char Peek() const;
  char PeekNext() const;
  char Advance();
  void ScanString();
  void ScanNumber();
  void ScanIdentifier();

  void AddToken(TokenType type);
  void AddToken(TokenType type, double num);
  void AddToken(TokenType type, string str);

  string source;
  int32_t start = 0;
  int32_t current = 0;
  int32_t line = 1;
  vector<Token> tokens;
  static map<string, TokenType> keywords;
};

#endif