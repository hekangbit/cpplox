#ifndef VALUE_H
#define VALUE_H

#include "common.h"

enum class ValueType {NIL_T, NUM_T, STR_T};

class LoxValue {
public:
  LoxValue() : type(ValueType::NIL_T) {}
  LoxValue(double num) : type(ValueType::NUM_T), num(num) {}
  LoxValue(string str) : type(ValueType::STR_T), str(str) {}
  ~LoxValue() {}

  bool IsNil() const { return type == ValueType::NIL_T; }
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