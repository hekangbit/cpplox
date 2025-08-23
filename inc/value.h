#ifndef VALUE_H
#define VALUE_H

#include <variant>
#include "common.h"

class LoxCallable;
using lox_callable_t = shared_ptr<LoxCallable>;

class Value {
public:
  Value() {} // NIL
  Value(double val) : data(val) {}
  Value(bool val) : data(val) {}
  Value(string val) : data(val) {}
  Value(lox_callable_t val) : data(val) {}

  bool isNil() const { return holds_alternative<monostate>(data); }
  bool isDouble() const { return holds_alternative<double>(data); }
  bool isBool() const { return holds_alternative<bool>(data); }
  bool isStr() const { return holds_alternative<std::string>(data); }
  bool isLoxCallable() const { return holds_alternative<lox_callable_t>(data); }

  double getDouble() const { return get<double>(data); }
  bool getBool() const { return get<bool>(data); }
  string getString() const { return get<string>(data); }
  lox_callable_t getLoxCallable() const { return get<lox_callable_t>(data); }

  Value operator+(const Value& other) const {
    if (isDouble() && other.isDouble()) {
        return Value(getDouble() + other.getDouble());
    }
    if (isStr() && other.isStr()) {
        return Value(getString() + other.getString());
    }
    if (isStr() && other.isDouble()) {
        return Value(getString() + to_string(other.getDouble()));
    }
    return Value();
  }

  Value operator-(const Value& other) const {
    if (isDouble() && other.isDouble()) {
        return Value(getDouble() - other.getDouble());
    }
    return Value();
  }

  Value operator*(const Value& other) const {
    if (isDouble() && other.isDouble()) {
        return Value(getDouble() * other.getDouble());
    }
    return Value();
  }

  Value operator/(const Value& other) const {
    if (isDouble() && other.isDouble()) {
        return Value(getDouble() / other.getDouble());
    }
    return Value();
  }

  Value operator>(const Value& other) const {
    if (isDouble() && other.isDouble()) {
        return getDouble() > other.getDouble();
    }
    return Value();
  }

  Value operator>=(const Value& other) const {
    if (isDouble() && other.isDouble()) {
        return getDouble() >= other.getDouble();
    }
    return Value();
  }

  Value operator<(const Value& other) const {
    if (isDouble() && other.isDouble()) {
        return getDouble() < other.getDouble();
    }
    return Value();
  }

  Value operator<=(const Value& other) const {
    if (isDouble() && other.isDouble()) {
        return getDouble() <= other.getDouble();
    }
    return Value();
  }

  Value operator==(const Value& other) const {
    if (isNil() && other.isNil()) {
      return true;
    }
    if (isNil()) {
      return false;
    }
    if (isDouble() && other.isDouble()) {
        return getDouble() == other.getDouble();
    }
    if (isStr() && other.isStr()) {
      return getString() == other.getString();
    }
    return false;
  }

  Value operator!=(const Value& other) const {
    Value value = operator==(other);
    return !(value.getBool());
  }

  string toString() const {
    if (isDouble()) {
      return to_string(getDouble());
    }
    if (isBool()) {
      return getBool() ? "true" : "false";
    }
    if (isStr()) {
      return getString();
    }
    return "nil";
  }

  friend std::ostream& operator<<(std::ostream& os, const Value& val) {
    if (val.isStr()) return os << val.getString();
    return os << val.toString();
  }
private:
  variant<monostate, int, double, bool, string, lox_callable_t> data;
};

#endif