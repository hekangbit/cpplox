#include "loxerror.h"

bool hadError = false;
bool hadRuntimeError = false;

void LoxError(int32_t line, const string message) {
  string res = "[line:";
  res = res + to_string(line);
  res = res + "], error: ";
  res = res + message;
  cout << res << endl;
  hadError = true;
}

void LoxError(token_t token, const string message) {
  int32_t line = token->line;
  string res = "[line:";
  res = res + to_string(line);
  res = res + "], at '" + token->lexeme;
  res = res + "', error: ";
  res = res + message;
  cout << res << endl;
  hadError = true;
}

void LoxRuntimeError(int32_t line, const string &message) {
  string res = "[line:";
  res = res + to_string(line);
  res = res + "], runtime error: ";
  res = res + message;
  cout << res << endl;
  hadRuntimeError = true;
}