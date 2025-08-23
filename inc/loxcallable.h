#ifndef LOXCALLABLE_H
#define LOXCALLABLE_H

#include "common.h"
#include "value.h"

class Interpreter;

class LoxCallable {
public:
  using CallFunction = function<Value(Interpreter&, const vector<Value>&)>;
  LoxCallable(int arity, CallFunction func, string name = "<native fn>") :
    arity(arity), func(func), name_(move(name)) {}
    virtual int Arity() { return arity; };
    virtual Value call(Interpreter &interpreter, vector<Value> &arguments) {
        return  func(interpreter, arguments);
    }
    string toString() const { return name_; }

private:
  int arity;
  CallFunction func;
  string name_;
};

#endif