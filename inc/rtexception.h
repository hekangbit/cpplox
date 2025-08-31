#ifndef RTEXCEPTION_H
#define RTEXCEPTION_H

#include "common.h"
#include "token.h"

class RuntimeException : public exception {
public:
  RuntimeException(token_t token, string message)
      : token(token), message(message) {}
  const char *what() const throw() {
    return message.c_str();
  }
  token_t token;
  string message;
};

#endif