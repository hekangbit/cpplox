#include "astprinter.h"
#include "common.h"
#include "error.h"
#include "expr.h"
#include "parser.h"
#include "scanner.h"

void run(string &str) {
  Scanner scanner(str);
  scanner.ScanTokens();
  cout << "finish scan" << endl;
  auto &tokens = scanner.GetTokens();
  cout << "tokens size = " << tokens.size() << endl;
  cout << "token result list:" << endl;
  for (auto &it : tokens) {
    cout << it.GetText() << endl;
  }

  Parser parser(scanner.GetTokens());

  {
    Expr *expr =
        new BinaryExpr(new NumberLiteralExpr(1), new Token(STAR, "+", 1),
                       new NumberLiteralExpr(2));
    AstPrinter printer;
    printer.Walk(*expr);
    cout << printer.Get();
  }

  {
    Expr *expr = new BinaryExpr(
        new UnaryExpr(new Token(MINUS, "-", 1), new NumberLiteralExpr(123)),
        new Token(STAR, "*", 1),
        new GroupingExpr(new NumberLiteralExpr(45.67)));
    AstPrinter printer;
    printer.Walk(*expr);
    cout << printer.Get();
  }

  {
    Expr *expr = parser.Parse();
    AstPrinter printer;
    if (expr) {
      printer.Walk(*expr);
      cout << printer.Get();
    }
  }
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
  char *buffer = new char[len];
  f.read(buffer, len);
  string str(buffer);
  f.close();
  delete[] buffer;
  run(str);
  if (hadError) {
    exit(65);
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
