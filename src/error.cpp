#include "error.h"

bool hadError = false;

void error(int32_t line, const string &message) {
  string res = "[line:";
  res = res + to_string(line);
  res = res + "], error:";
  res = res + message;
  cout << res << endl;
}