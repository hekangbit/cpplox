#include "scanner.h"
#include "error.h"

bool Scanner::IsAtEnd() const { return current >= source.size(); }

char Scanner::Advance() {
  assert(current <= source.size());
  return source[current++];
}

void Scanner::AddToken(TokenType type) {
  string text = source.substr(start, current - start);
  cout << source << endl;
  cout << "start:" << start << ", end:" << current << ", text:" << text << endl;
  Token token(type, text, line);
  tokens.push_back(token);
}

void Scanner::AddToken(TokenType type, int32_t num) {
  string text = source.substr(start, current);
  Token token(type, text, num, line);
  tokens.push_back(token);
}

void Scanner::AddToken(TokenType type, string str) {
  string text = source.substr(start, current);
  Token token(type, text, str, line);
  tokens.push_back(token);
}

void Scanner::ScanToken() {
  char c = Advance();
  switch (c) {
  case '(':
    AddToken(LEFT_PAREN);
    break;
  case ')':
    AddToken(RIGHT_PAREN);
    break;
  case '{':
    AddToken(LEFT_BRACE);
    break;
  case '}':
    AddToken(RIGHT_BRACE);
    break;
  case ',':
    AddToken(COMMA);
    break;
  case '.':
    AddToken(DOT);
    break;
  case '-':
    AddToken(MINUS);
    break;
  case '+':
    AddToken(PLUS);
    break;
  case ';':
    AddToken(SEMICOLON);
    break;
  case '*':
    AddToken(STAR);
    break;
  case '\n':
    line++;
    break;
  default:
    error(line, "Unexpected char.");
    break;
  }
}

void Scanner::ScanTokens() {
  while (!IsAtEnd()) {
    start = current;
    // cout << "start:" << start << endl;
    ScanToken();
  }
}

vector<Token> &Scanner::GetTokens() { return tokens; }