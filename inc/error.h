#ifndef ERROR_H
#define ERROR_H

#include "common.h"

extern bool hadError;
void error(int32_t line, const string &message);

#endif