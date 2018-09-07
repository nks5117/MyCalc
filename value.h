// value.h
// Copyright (c) 2018 Ni Kesu. All rights reserved.

#ifndef __VALUE_H__
#define __VALUE_H__
#include <string>
#include "bigint.h"
#include "token.h"
enum ValueType {
  V_BIGINT, V_DOUBLE, V_NA
};

class Value {
private:
  std::string name_;
  ValueType type_;
  BigInt *intValue_;
  double *doubleValue_;
  bool isLvalue_;
public:
  Value(std::string name = "", ValueType type = V_NA, BigInt *intValue = nullptr,
        double *doubleValue = nullptr, bool isLvalue = true);
  Value(const BigInt &n);
  Value(const double &n);
  Value(const bool &n);
  Value(const Value &n);
  ~Value();
  Value &operator=(const Value &n);
  ValueType type() const;
  BigInt intValue() const;
  double doubleValue() const;
  bool isLvalue() const;
  bool setType(ValueType type);
  bool setValue(const BigInt &n);
  bool setValue(const double &n);
};
Value toValue(const Token &n);
#endif