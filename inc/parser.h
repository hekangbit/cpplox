#ifndef PARSER_H
#define PARSER_H
#include "common.h"
#include "token.h"
#include <vector>

class Parser {
public:
  Parser(vector<Token> tokens) : tokens(tokens){};

private:
  vector<Token> tokens;
  int32_t current = 0;
};

#endif