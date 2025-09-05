#ifndef VALUE_H
#define VALUE_H

#include "common.h"
#include <variant>

class LoxInstance;
using lox_instance_t = shared_ptr<LoxInstance>;
class LoxCallable;
using lox_callable_t = shared_ptr<LoxCallable>;

class Value {
public:
  Value() {} // NIL
  Value(double val) : data(val) {}
  Value(bool val) : data(val) {}
  Value(string val) : data(val) {}
  Value(lox_callable_t val) : data(val) {}
  Value(lox_instance_t val) : data(val) {}

  bool isNil() const {
    return holds_alternative<monostate>(data);
  }
  bool isDouble() const {
    return holds_alternative<double>(data);
  }
  bool isBool() const {
    return holds_alternative<bool>(data);
  }
  bool isStr() const {
    return holds_alternative<std::string>(data);
  }
  bool isLoxCallable() const {
    return holds_alternative<lox_callable_t>(data);
  }
  bool isLoxInstance() const {
    return holds_alternative<lox_instance_t>(data);
  }

  double getDouble() const;
  bool getBool() const;
  string getString() const;
  lox_callable_t getLoxCallable() const;
  lox_instance_t getLoxInstance() const;

  Value operator+(const Value &other) const;
  Value operator-(const Value &other) const;
  Value operator*(const Value &other) const;
  Value operator/(const Value &other) const;
  Value operator>(const Value &other) const;
  Value operator>=(const Value &other) const;
  Value operator<(const Value &other) const;
  Value operator<=(const Value &other) const;
  Value operator==(const Value &other) const;
  Value operator!=(const Value &other) const;
  string toString() const;

  friend std::ostream &operator<<(std::ostream &os, const Value &val) {
    if (val.isStr())
      return os << val.getString();
    return os << val.toString();
  }

private:
  variant<monostate, int, double, bool, string, lox_callable_t, lox_instance_t>
      data;
};

#endif