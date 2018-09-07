// value.cpp
// Copyright (c) 2018 Ni Kesu. All rights reserved.

#include <iostream>
#include <cassert>
#include "value.h"
#include "variable.h"
using std::string;
Value::Value(std::string name, ValueType type, BigInt *intValue,
             double *doubleValue, bool isLvalue)
  : name_(name), type_(type), intValue_(intValue),
    doubleValue_(doubleValue), isLvalue_(isLvalue)
{

}

Value::Value(const BigInt & n)
{
  type_ = V_BIGINT;
  intValue_ = new BigInt(n);
  doubleValue_ = nullptr;
  isLvalue_ = false;
}

Value::Value(const double & n)
{
  type_ = V_DOUBLE;
  intValue_ = nullptr;
  doubleValue_ = new double(n);
  isLvalue_ = false;
}

Value::Value(const bool & n)
{
  intValue_ = nullptr;
  doubleValue_ = nullptr;
  *this = static_cast<BigInt>(n);
}

Value::Value(const Value & n) : type_(n.type_), isLvalue_(n.isLvalue_)
{
  if (isLvalue_) {
    intValue_ = n.intValue_;
    doubleValue_ = n.doubleValue_;
  }
  else {
    if (type_ == V_BIGINT) {
      intValue_ = new BigInt(*n.intValue_);
      doubleValue_ = nullptr;
    }
    else if (type_ == V_DOUBLE) {
      intValue_ = nullptr;
      doubleValue_ = new double(*n.doubleValue_);
    }
  }
}

Value::~Value()
{
  if (!isLvalue_) {
    delete intValue_;
    delete doubleValue_;
  }
}

Value & Value::operator=(const Value & n)
{
  if (this == &n) {
    return *this;
  }
  else {
    if (!isLvalue_) {
      delete intValue_;
      delete doubleValue_;
    }
    type_ = n.type_;
    isLvalue_ = n.isLvalue_;
    if (isLvalue_) {
      intValue_ = n.intValue_;
      doubleValue_ = n.doubleValue_;
    }
    else {
      if (type_ == V_BIGINT) {
        intValue_ = new BigInt(*n.intValue_);
        doubleValue_ = nullptr;
      }
      else if (type_ == V_DOUBLE) {
        intValue_ = nullptr;
        doubleValue_ = new double(*n.doubleValue_);
      }
    }
    return *this;
  }
}

ValueType Value::type() const
{
  return type_;
}

BigInt Value::intValue() const
{
  return *intValue_;
}

double Value::doubleValue() const
{
  return *doubleValue_;
}

bool Value::isLvalue() const
{
  return isLvalue_;
}

bool Value::setType(ValueType type)
{
  type_ = type;
  return true;
}

bool Value::setValue(const BigInt & n)
{
  if (!isLvalue_) {
    std::cerr << " in setValue: n is not a lvalue!";
    exit(-1);
  }
  
  if (type_ == V_BIGINT) {
    *intValue_ = n;
  }
  else if (type_ == V_DOUBLE) {
    *doubleValue_ = static_cast<double>(n);
  }
  else {
    std::cerr << "in setValue: wrong type";
    exit(-1);
  }

  return true;
}

bool Value::setValue(const double & n)
{
  if (!isLvalue_) {
    std::cerr << " in setValue: n is not a lvalue!";
    exit(-1);
  }
  
  if (type_ == V_BIGINT) {
    *intValue_ = static_cast<BigInt>(n);
  }
  else if (type_ == V_DOUBLE) {
    *doubleValue_ = n;
  }
  else {
    std::cerr << "in setValue: wrong type";
    exit(-1);
  }

  return true;
}

Value toValue(const Token & n)
{
  if (n.type == T_LITERAL) {
    for (const auto &c : n.str) {
      if (c == '.') {
        return Value(strtod(n.str.c_str(), nullptr));
      }
    }
    return Value(BigInt(n.str));
  }
  else if (n.type == T_IDENTIFIER) {
    for (auto & v : variableList) {
      if (v.name() == n.str) {
        return Value(v.name(), v.type(), v.intValue(), v.doubleValue(), true);
      }
    }
    variableList.push_back(Variable(n.str, V_NA, ZERO, 0.0));
    return Value(variableList.back().name(),
                 variableList.back().type(),
                 variableList.back().intValue(),
                 variableList.back().doubleValue(),
                 true);
  }
  else {
    // TODO
    return Value(ZERO);
  }
}
