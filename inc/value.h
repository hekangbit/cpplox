#ifndef VALUE_H
#define VALUE_H

#include "common.h"

enum class ValueType { NIL_T, NUM_T, STR_T };

class LoxValue {
public:
  LoxValue() : type(ValueType::NIL_T), str("nil") {}
  LoxValue(double num) : type(ValueType::NUM_T), num(num) {}
  LoxValue(string str) : type(ValueType::STR_T), str(str) {}
  LoxValue(const LoxValue &other) {
    this->type = other.type;
    if (other.type == ValueType::NUM_T) {
      this->num = other.num;
    } else if (other.type == ValueType::STR_T) {
      this->str = other.str;
    }
  }
  LoxValue &operator=(const LoxValue &other) {
    this->type = other.type;
    if (other.type == ValueType::NUM_T) {
      this->num = other.num;
    } else if (other.type == ValueType::STR_T) {
      this->str = other.str;
    }
    return *this;
  }
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