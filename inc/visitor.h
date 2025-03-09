#ifndef VISITOR_H
#define VISITOR_H

#include "value.h"

class NumberLiteralExpr;
class StringLiteralExpr;
class UnaryExpr;
class BinaryExpr;
class GroupingExpr;
class NullLiteralExpr;
class BoolLiteralExpr;
class VariableExpr;
class AssignExpr;
class ExprStmt;
class PrintStmt;
class VarStmt;

class Visitor {
public:
  virtual ~Visitor() {}
  virtual LoxValue Visit(NumberLiteralExpr &expr) = 0;
  virtual LoxValue Visit(StringLiteralExpr &expr) = 0;
  virtual LoxValue Visit(UnaryExpr &expr) = 0;
  virtual LoxValue Visit(BinaryExpr &expr) = 0;
  virtual LoxValue Visit(GroupingExpr &expr) = 0;
  virtual LoxValue Visit(NullLiteralExpr &expr) = 0;
  virtual LoxValue Visit(BoolLiteralExpr &expr) = 0;
  virtual LoxValue Visit(VariableExpr &expr) = 0;
  virtual LoxValue Visit(AssignExpr &expr) = 0;
  virtual void Visit(ExprStmt &stmt) = 0;
  virtual void Visit(PrintStmt &stmt) = 0;
  virtual void Visit(VarStmt &stmt) = 0;
};

#endif