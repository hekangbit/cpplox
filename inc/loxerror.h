#ifndef LOXERROR_H
#define LOXERROR_H

#include "common.h"
#include "token.h"

extern bool hadError;
extern bool hadRuntimeError;
void LoxError(int32_t line, const string message);
void LoxError(token_t token, const string message);
void LoxRuntimeError(int32_t line, const string &message);

#endif