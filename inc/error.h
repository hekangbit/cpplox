#ifndef ERROR_H
#define ERROR_H

#include "common.h"

extern bool hadError;
extern bool hadRuntimeError;
void error(int32_t line, const string &message);
void runtimeError(int32_t line, const string &message);

#endif