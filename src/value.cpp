#include "value.h"
#include "loxcallable.h"
#include "loxclass.h"
#include "loxinstance.h"

double Value::getDouble() const {
  return get<double>(data);
}

bool Value::getBool() const {
  return get<bool>(data);
}

string Value::getString() const {
  return get<string>(data);
}

lox_callable_t Value::getLoxCallable() const {
  if (isLoxClass()) {
    return static_pointer_cast<LoxCallable>(getLoxClass());
  }
  return get<lox_callable_t>(data);
}

lox_class_t Value::getLoxClass() const {
  return get<lox_class_t>(data);
}

lox_instance_t Value::getLoxInstance() const {
  return get<lox_instance_t>(data);
}

Value Value::operator+(const Value &other) const {
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

Value Value::operator-(const Value &other) const {
  if (isDouble() && other.isDouble()) {
    return Value(getDouble() - other.getDouble());
  }
  return Value();
}

Value Value::operator*(const Value &other) const {
  if (isDouble() && other.isDouble()) {
    return Value(getDouble() * other.getDouble());
  }
  return Value();
}

Value Value::operator/(const Value &other) const {
  if (isDouble() && other.isDouble()) {
    return Value(getDouble() / other.getDouble());
  }
  return Value();
}

Value Value::operator>(const Value &other) const {
  if (isDouble() && other.isDouble()) {
    return getDouble() > other.getDouble();
  }
  return Value();
}

Value Value::operator>=(const Value &other) const {
  if (isDouble() && other.isDouble()) {
    return getDouble() >= other.getDouble();
  }
  return Value();
}

Value Value::operator<(const Value &other) const {
  if (isDouble() && other.isDouble()) {
    return getDouble() < other.getDouble();
  }
  return Value();
}

Value Value::operator<=(const Value &other) const {
  if (isDouble() && other.isDouble()) {
    return getDouble() <= other.getDouble();
  }
  return Value();
}

Value Value::operator==(const Value &other) const {
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

Value Value::operator!=(const Value &other) const {
  Value value = operator==(other);
  return !(value.getBool());
}

string Value::toString() const {
  if (isDouble()) {
    return to_string(getDouble());
  }
  if (isBool()) {
    return getBool() ? "true" : "false";
  }
  if (isStr()) {
    return getString();
  }
  if (isLoxCallable()) {
    auto func = getLoxCallable();
    return func->toString();
  }
  if (isLoxClass()) {
    auto klass = getLoxClass();
    return klass->toString();
  }
  if (isLoxInstance()) {
    auto instance = getLoxInstance();
    return instance->toString();
  }
  return "nil";
}