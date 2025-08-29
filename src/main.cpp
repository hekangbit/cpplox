#include "astprinter.h"
#include "common.h"
#include "loxerror.h"
#include "expr.h"
#include "resolver.h"
#include "interpreter.h"
#include "parser.h"
#include "scanner.h"

void run(string &str) {
  Scanner scanner(str);
  scanner.ScanTokens();

  Parser parser(scanner.GetTokens());
  if (hadError) {
    return;
  }

  vector<stmt_t> statements = parser.Parse();

  Interpreter interpreter;
  Resolver resolver(&interpreter);
  resolver.Resolve(statements);
  if (hadError) {
    return;
  }
  interpreter.Interpret(statements);
}

int RunFile(string filename) {
  ifstream f(filename.c_str());
  if (!f) {
    cout << "Error open file hhh  !" << endl;
    exit(66);
  }
  f.seekg(0, f.end);
  int64_t len = f.tellg();
  f.seekg(0, f.beg);
  char *buffer = new char[len + 1];
  memset(buffer, 0x00, len + 1);
  f.read(buffer, len);
  string str(buffer);
  f.close();
  delete[] buffer;
  run(str);
  if (hadError) {
    exit(65);
  }
  if (hadRuntimeError) {
    exit(70);
  }
  return 0;
}

int RunPrompt() {
  string line;
  // run(line);
  hadError = false;
  return 0;
}

int main(int argc, char **args) {
  cout << "CPP Lox Lang VM" << endl;
  if (argc > 2) {
    cout << "Wrong arg num" << endl;
    exit(64);
  } else if (argc == 2) {
    RunFile(args[1]);
  } else {
    RunPrompt();
  }

  return 0;
}
