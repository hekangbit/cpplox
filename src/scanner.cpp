#include "scanner.h"
#include "error.h"

map<string, TokenType> Scanner::keywords = {
    {"and", AND},     {"or", OR},      {"true", TRUE},   {"false", FALSE},
    {"if", IF},       {"else", _ELSE}, {"for", FOR},     {"while", WHILE},
    {"var", VAR},     {"this", _THIS}, {"super", SUPER}, {"return", RETURN},
    {"print", PRINT}, {"fun", FUN},    {"class", CLASS}, {"nil", NIL}};

bool Scanner::IsAtEnd() const { return current >= source.size(); }

bool Scanner::IsDigit(char c) const { return c >= '0' && c <= '9'; }

bool Scanner::IsAlpha(char c) const {
  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'));
}

bool Scanner::IsAlphaNumeric(char c) const { return IsAlpha(c) || IsDigit(c); }

bool Scanner::Match(char c) {
  if (IsAtEnd()) {
    return false;
  }
  if (source[current] != c) {
    return false;
  }
  current++;
  return true;
}

char Scanner::Peek() const {
  if (IsAtEnd()) {
    return 0;
  }
  return source[current];
}

char Scanner::Advance() {
  assert(current <= source.size());
  return source[current++];
}

void Scanner::AddToken(TokenType type) {
  string text = source.substr(start, current - start);
  Token token(type, text, line);
  tokens.push_back(token);
}

void Scanner::AddToken(TokenType type, int32_t num) {
  string text = source.substr(start, current - start);
  Token token(type, text, num, line);
  tokens.push_back(token);
}

void Scanner::AddToken(TokenType type, string str) {
  string text = source.substr(start, current - start);
  Token token(type, text, str, line);
  tokens.push_back(token);
}

void Scanner::ScanString() {
  // TODO: make lexeme to string token
  while (Peek() != '"' && !IsAtEnd()) {
    if (Peek() == '\n')
      line++;
    Advance();
  }
  if (IsAtEnd()) {
    error(line, "Invalid literal string");
    return;
  }
  Advance();
  string literal = source.substr(start + 1, current - start - 2);
  AddToken(STRING, literal);
}

void Scanner::ScanNumber() {
  // TODO: make lexeme to number token
}

void Scanner::ScanIdentifier() {
  // TODO: make lexeme to identifier or keyword
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
  case '!':
    AddToken(Match('=') ? BANG_EQUAL : BANG);
    break;
  case '=':
    AddToken(Match('=') ? EQUAL : ASSIGN);
    break;
  case '<':
    AddToken(Match('=') ? LESS_EQUAL : LESS);
    break;
  case '>':
    AddToken(Match('=') ? GREATER_EQUAL : GREATER);
    break;
  case '/':
    if (Match('/')) {
      while (Peek() != '\n' && !IsAtEnd()) {
        Advance();
      }
    } else {
      AddToken(SLASH);
    }
    break;
  case '\n':
    line++;
    break;
  case ' ':
  case '\r':
  case '\t':
    break;
  case '"':
    ScanString();
    break;
  default:
    if (IsDigit(c)) {
      ScanNumber();
    } else if (IsAlpha(c)) {
      ScanIdentifier();
    } else {
      error(line, "Unexpected char.");
    }
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