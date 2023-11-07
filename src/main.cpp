#include "common.h"

using namespace std;

bool hadError = false;

void error(int line, string message) {
  string res = "[line:";
  res = res + to_string(line);
  res = res + "], error:";
  res = res + message;
  cout << res << endl;
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
  string s(buffer);
  cout << s << endl;
  f.close();
  delete [] buffer;
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

