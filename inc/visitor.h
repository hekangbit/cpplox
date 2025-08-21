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
class LogicalExpr;
class AssignExpr;
class ExprStmt;
class PrintStmt;
class BlockStmt;
class VarStmt;
class IfStmt;
class WhileStmt;
class BreakStmt;

class Visitor {
public:
  virtual ~Visitor() {}
  virtual Value Visit(NumberLiteralExpr &expr) = 0;
  virtual Value Visit(StringLiteralExpr &expr) = 0;
  virtual Value Visit(UnaryExpr &expr) = 0;
  virtual Value Visit(BinaryExpr &expr) = 0;
  virtual Value Visit(GroupingExpr &expr) = 0;
  virtual Value Visit(NullLiteralExpr &expr) = 0;
  virtual Value Visit(BoolLiteralExpr &expr) = 0;
  virtual Value Visit(LogicalExpr &expr) = 0;
  virtual Value Visit(VariableExpr &expr) = 0;
  virtual Value Visit(AssignExpr &expr) = 0;
  virtual void Visit(ExprStmt &stmt) = 0;
  virtual void Visit(PrintStmt &stmt) = 0;
  virtual void Visit(BlockStmt &stmt) = 0;
  virtual void Visit(VarStmt &stmt) = 0;
  virtual void Visit(IfStmt &stmt) = 0;
  virtual void Visit(WhileStmt &stmt) = 0;
  virtual void Visit(BreakStmt &stmt) = 0;
};

#endif