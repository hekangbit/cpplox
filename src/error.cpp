#include "error.h"

bool hadError = false;
bool hadRuntimeError = false;

void error(int32_t line, const string &message) {
  string res = "[line:";
  res = res + to_string(line);
  res = res + "], error: ";
  res = res + message;
  cout << res << endl;
  hadError = true;
}

void runtimeError(int32_t line, const string &message) {
  string res = "[line:";
  res = res + to_string(line);
  res = res + "], runtime error: ";
  res = res + message;
  cout << res << endl;
  hadRuntimeError = true;
}