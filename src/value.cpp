#include "value.h"
#include "loxcallable.h"

Value Value::operator+(const Value& other) const {
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

Value Value::operator-(const Value& other) const {
if (isDouble() && other.isDouble()) {
    return Value(getDouble() - other.getDouble());
}
return Value();
}

Value Value::operator*(const Value& other) const {
if (isDouble() && other.isDouble()) {
    return Value(getDouble() * other.getDouble());
}
return Value();
}

Value Value::operator/(const Value& other) const {
if (isDouble() && other.isDouble()) {
    return Value(getDouble() / other.getDouble());
}
return Value();
}

Value Value::operator>(const Value& other) const {
if (isDouble() && other.isDouble()) {
    return getDouble() > other.getDouble();
}
return Value();
}

Value Value::operator>=(const Value& other) const {
if (isDouble() && other.isDouble()) {
    return getDouble() >= other.getDouble();
}
return Value();
}

Value Value::operator<(const Value& other) const {
if (isDouble() && other.isDouble()) {
    return getDouble() < other.getDouble();
}
return Value();
}

Value Value::operator<=(const Value& other) const {
if (isDouble() && other.isDouble()) {
    return getDouble() <= other.getDouble();
}
return Value();
}

Value Value::operator==(const Value& other) const {
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

Value Value::operator!=(const Value& other) const {
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
    return "nil";
}