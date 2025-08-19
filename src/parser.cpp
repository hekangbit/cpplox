#include "parser.h"

bool Parser::IsAtEnd() { return current >= tokens.size(); }

bool Parser::Check(TokenType type) {
  if (IsAtEnd()) {
    return false;
  }
  return Peek()->type == type;
}

bool Parser::Match(initializer_list<TokenType> token_types) {
  for (auto &it : token_types) {
    if (Check(it)) {
      Advance();
      return true;
    }
  }
  return false;
}

token_t Parser::Previous() {
  if (current == 0) {
    return nullptr;
  }
  return (&(tokens[current - 1]));
}

token_t Parser::Advance() {
  if (IsAtEnd()) {
    return Previous();
  }
  current++;
  return Previous();
}

token_t Parser::Consume(TokenType type, string message) {
  if (Check(type)) {
    return Advance();
  }
  throw Error(Peek(), message);
}

token_t Parser::Peek() {
  if (IsAtEnd()) {
    return nullptr;
  }
  return (&(tokens[current]));
}

ParserException Parser::Error(token_t token, string message) {
  if (token == nullptr) {
    message = "at the end, " + message;
    error(tokens.size() ? tokens.back().line : 0, message);
  } else {
    message = "found token <" + token->lexeme + ">, " + message;
    error(token->line, message);
  }
  return ParserException();
}

void Parser::Synchronize() {
  Advance();
  while (!IsAtEnd()) {
    token_t curr_token = Peek();
    if (curr_token->type == SEMICOLON) {
      return;
    }
    switch (curr_token->type) {
      case VAR:
      case IF:
      case FOR:
      case WHILE:
      case FUN:
      case CLASS:
      case RETURN:
      case PRINT:
        return;
      default:
        break;
    }
    Advance();
  }
}

expr_t Parser::Assignment() {
  expr_t expr = Equality();

  if (Match({EQUAL})) {
    token_t equal_token = Previous();
    expr_t value = Assignment();
    auto var = dynamic_pointer_cast<VariableExpr>(expr);
    if (var) {
      return expr_t(new AssignExpr(var->token, value));
    }
    Error(equal_token, "Invalid assignment target.");
  }
  return expr;
}

expr_t Parser::Equality() {
  expr_t expr = Comparison();
  while (Match({EQUAL_EQUAL, BANG_EQUAL})) {
    token_t op = Previous();
    expr_t right = Comparison();
    expr = expr_t(new BinaryExpr(expr, op, right));
  }
  return expr;
}

expr_t Parser::Comparison() {
  expr_t expr = Term();
  while (Match({LESS_EQUAL, LESS, GREATER_EQUAL, GREATER})) {
    token_t op = Previous();
    expr_t right = Term();
    expr = expr_t(new BinaryExpr(expr, op, right));
  }
  return expr;
}

expr_t Parser::Term() {
  expr_t expr = Factor();
  while (Match({PLUS, MINUS})) {
    token_t op = Previous();
    expr_t right = Factor();
    expr = expr_t(new BinaryExpr(expr, op, right));
  }
  return expr;
}

expr_t Parser::Factor() {
  expr_t expr = Unary();
  while (Match({STAR, SLASH})) {
    token_t op = Previous();
    expr_t right = Unary();
    expr = expr_t(new BinaryExpr(expr, op, right));
  }
  return expr;
}

expr_t Parser::Unary() {
  if (Match({MINUS, BANG})) {
    token_t op = Previous();
    expr_t expr = Unary();
    return expr_t(new UnaryExpr(op, expr));
  }
  return Primary();
}

expr_t Parser::Primary() {
  if (Match({TRUE})) {
    return expr_t(new BoolLiteralExpr(true));
  }
  if (Match({FALSE})) {
    return expr_t(new BoolLiteralExpr(false));
  }
  if (Match({NIL})) {
    return expr_t(new NullLiteralExpr());
  }
  if (Match({NUMBER})) {
    return expr_t(new NumberLiteralExpr(Previous()->literalNum));
  }
  if (Match({STRING})) {
    return expr_t(new StringLiteralExpr(Previous()->literalStr));
  }
  if (Match({LEFT_PAREN})) {
    expr_t expr = Expression();
    Consume(RIGHT_PAREN, "expect ')' after expression.");
    return expr_t(new GroupingExpr(expr));
  }
  if (Match({IDENTIFIER})) {
    token_t t = Previous();
    return expr_t(new VariableExpr(t));
  }
  throw Error(Peek(), "expect expr.");
  return nullptr;
}

expr_t Parser::Expression() { return Assignment(); }

stmt_t Parser::printStatement() {
  token_t t = Peek();
  expr_t expr = Expression();
  stmt_t stmt(new PrintStmt(t, expr));
  Consume(SEMICOLON, "Expect ';' in print statement.");
  return stmt_t(stmt);
}

stmt_t Parser::ifStatement() {
  Consume(LEFT_PAREN, "Expect '(' after 'if'.");
  expr_t condition = Expression();
  Consume(RIGHT_PAREN, "Expect '(' after ')'.");
  stmt_t thenStmt = Statement();
  stmt_t elseStmt = nullptr;
  if (Match({_ELSE})) {
    elseStmt = Statement();
  }
  return stmt_t(new IfStmt(condition, thenStmt, elseStmt));
}


stmt_t Parser::blockStatement() {
  vector<stmt_t> stmts;
  while (!Check(RIGHT_BRACE) && !IsAtEnd()) {
    stmt_t stmt = Declaration();
    if (stmt) {
      stmts.push_back(stmt);
    }
  }
  Consume(RIGHT_BRACE, "Expect '}' in block statement.");
  return stmt_t(new BlockStmt(stmts));
}

stmt_t Parser::expressionStatemenmt() {
  stmt_t stmt(new ExprStmt(Expression()));
  Consume(SEMICOLON, "Expect ';' in expr statement.");
  return stmt;
}

stmt_t Parser::Statement() {
  if (Match({PRINT})) {
    return printStatement();
  } else if (Match({LEFT_BRACE})) {
    return blockStatement();
  } else if (Match({IF})) {
    return ifStatement();
  }
  return expressionStatemenmt();
}

stmt_t Parser::VarDeclaration() {
  token_t token = Consume(IDENTIFIER, "Expect a variable name.");
  expr_t initializer = nullptr;
  if (Match({EQUAL})) {
    initializer = Expression();
  }
  Consume(SEMICOLON, "Expect ';' in var declaration statement.");
  return stmt_t(new VarStmt(token, initializer));
}

stmt_t Parser::Declaration() {
  try {
    if (Match({VAR})) {
      return VarDeclaration();
    }
    return Statement();
  } catch (const ParserException &e) {
    cout << "Catch Parser exception." << endl;
    cerr << e.what() << endl;
    Synchronize();
    return nullptr;
  }
}

vector<stmt_t> Parser::Parse() {
  vector<stmt_t> statements;
  while (!IsAtEnd()) {
    stmt_t stmt = Declaration();
    if (stmt) {
      statements.push_back(stmt);
    }
  }
  return statements;
}