#ifndef VALUE_H
#define VALUE_H

#include "common.h"

enum class ValueType {NUM_T, STR_T};

class LoxValue {
public:
  LoxValue(double num) : type(ValueType::NUM_T), num(num) {}
  LoxValue(string str) : type(ValueType::STR_T), str(str) {}
  ~LoxValue() {}

  bool IsNum() const { return type == ValueType::NUM_T; }
  bool IsStr() const { return type == ValueType::STR_T; }
  double GetNum() const { return num; }
  string GetStr() const { return str; }

private:
  ValueType type;
  union {
    double num;
    string str;
  };
};

#endif