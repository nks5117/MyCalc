// variable.h
// Copyright (c) 2018 Ni Kesu. All rights reserved.

#ifndef __VARIABLE_H__
#define __VARIABLE_H__
#include <vector>
#include <string>
#include "value.h"
#include "bigint.h"

class Variable {
private:
  std::string name_;
  ValueType type_;
  BigInt intValue_;
  double doubleValue_;
public:
  Variable(const std::string &name,
           ValueType type,
           const BigInt &intValue,
           double doubleValue)
    : name_(name), type_(type), intValue_(intValue), doubleValue_(doubleValue)
  {}
  std::string name() const { return name_; }
  ValueType type() const { return type_; }
  BigInt *intValue() { return &intValue_; }
  double *doubleValue() { return &doubleValue_; }
  bool setType(ValueType type) { type_ = type; return true; }
};

extern std::vector<Variable> variableList;

#endif // !__VARIABLE_H__