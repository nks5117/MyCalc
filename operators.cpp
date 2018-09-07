// operators.cpp
// Copyright (c) 2018 Ni Kesu. All rights reserved.

#include <iostream>
#include <cmath>
#include "operators.h"
#include "variable.h"
Value factorial(const Value & n)
{
  if (n.type() == V_BIGINT) {
    BigInt r = ONE;
    for (BigInt i = n.intValue(); i > ZERO; --i) {
      r *= i;
    }
    return Value(r);
  }
  else {
    std::cerr << "non-integral factorial";
    exit(-1);
  }
}

Value power(const Value & lhs, const Value & rhs)
{
  if (lhs.type() == V_BIGINT && rhs.type() == V_BIGINT) {
    return Value(pow(lhs.intValue(), rhs.intValue()));
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_BIGINT) {
    return Value(pow(lhs.doubleValue(), static_cast<double>(rhs.intValue())));
  }
  else if (lhs.type() == V_BIGINT && rhs.type() == V_DOUBLE) {
    return Value(pow(static_cast<double>(lhs.intValue()), rhs.doubleValue()));
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_DOUBLE) {
    return Value(pow(lhs.doubleValue(), rhs.doubleValue()));
  }
  else {
    std::cerr << "error";
    exit(-1);
  }
}

Value assign(Value & lhs, const Value & rhs)
{
  if (lhs.isLvalue() == false) {
    std::cerr << "left operand is not a modifiable lvalue";
    exit(-1);
  }
  
  lhs.setType(rhs.type());
  for (auto & v : variableList) {
    v.setType(rhs.type());
  }
  
  if (rhs.type() == V_BIGINT) {
    lhs.setValue(rhs.intValue());
  }
  else if (rhs.type() == V_DOUBLE) {
    lhs.setValue(rhs.doubleValue());
  }
  return lhs;
}

Value plus(const Value & lhs, const Value & rhs)
{
  if (lhs.type() == V_BIGINT && rhs.type() == V_BIGINT) {
    return Value(lhs.intValue() + rhs.intValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_BIGINT) {
    return Value(lhs.doubleValue() + static_cast<double>(rhs.intValue()));
  }
  else if (lhs.type() == V_BIGINT && rhs.type() == V_DOUBLE) {
    return Value(static_cast<double>(lhs.intValue()) + rhs.doubleValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_DOUBLE) {
    return Value(lhs.doubleValue() + rhs.doubleValue());
  }
  else {
    std::cerr << "error";
    exit(-1);
  }
}

Value minus(const Value & lhs, const Value & rhs)
{
  if (lhs.type() == V_BIGINT && rhs.type() == V_BIGINT) {
    return Value(lhs.intValue() - rhs.intValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_BIGINT) {
    return Value(lhs.doubleValue() - static_cast<double>(rhs.intValue()));
  }
  else if (lhs.type() == V_BIGINT && rhs.type() == V_DOUBLE) {
    return Value(static_cast<double>(lhs.intValue()) - rhs.doubleValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_DOUBLE) {
    return Value(lhs.doubleValue() - rhs.doubleValue());
  }
  else {
    std::cerr << "error";
    exit(-1);
  }
}

Value multiply(const Value & lhs, const Value & rhs)
{
  if (lhs.type() == V_BIGINT && rhs.type() == V_BIGINT) {
    return Value(lhs.intValue() * rhs.intValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_BIGINT) {
    return Value(lhs.doubleValue() * static_cast<double>(rhs.intValue()));
  }
  else if (lhs.type() == V_BIGINT && rhs.type() == V_DOUBLE) {
    return Value(static_cast<double>(lhs.intValue()) * rhs.doubleValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_DOUBLE) {
    return Value(lhs.doubleValue() * rhs.doubleValue());
  }
  else {
    std::cerr << "error";
    exit(-1);
  }
}

Value divide(const Value & lhs, const Value & rhs)
{
  if (lhs.type() == V_BIGINT && rhs.type() == V_BIGINT) {
    return Value(lhs.intValue() / rhs.intValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_BIGINT) {
    return Value(lhs.doubleValue() / static_cast<double>(rhs.intValue()));
  }
  else if (lhs.type() == V_BIGINT && rhs.type() == V_DOUBLE) {
    return Value(static_cast<double>(lhs.intValue()) / rhs.doubleValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_DOUBLE) {
    return Value(lhs.doubleValue() / rhs.doubleValue());
  }
  else {
    std::cerr << "error";
    exit(-1);
  }
}

Value modulo(const Value & lhs, const Value & rhs)
{
  if (lhs.type() == V_BIGINT && rhs.type() == V_BIGINT) {
    return Value(lhs.intValue()
                 - lhs.intValue() / rhs.intValue() * rhs.intValue());
  }
  else {
    std::cerr << "non-integral modulo";
    exit(-1);
  }
}

Value equal(const Value & lhs, const Value & rhs)
{
  if (lhs.type() == V_BIGINT && rhs.type() == V_BIGINT) {
    return Value(lhs.intValue() == rhs.intValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_BIGINT) {
    return Value(lhs.doubleValue() == static_cast<double>(rhs.intValue()));
  }
  else if (lhs.type() == V_BIGINT && rhs.type() == V_DOUBLE) {
    return Value(static_cast<double>(lhs.intValue()) == rhs.doubleValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_DOUBLE) {
    return Value(lhs.doubleValue() == rhs.doubleValue());
  }
  else {
    std::cerr << "error";
    exit(-1);
  }
}

Value notequal(const Value & lhs, const Value & rhs)
{
  if (lhs.type() == V_BIGINT && rhs.type() == V_BIGINT) {
    return Value(lhs.intValue() != rhs.intValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_BIGINT) {
    return Value(lhs.doubleValue() != static_cast<double>(rhs.intValue()));
  }
  else if (lhs.type() == V_BIGINT && rhs.type() == V_DOUBLE) {
    return Value(static_cast<double>(lhs.intValue()) != rhs.doubleValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_DOUBLE) {
    return Value(lhs.doubleValue() != rhs.doubleValue());
  }
  else {
    std::cerr << "error";
    exit(-1);
  }
}

Value lessthan(const Value & lhs, const Value & rhs)
{
  if (lhs.type() == V_BIGINT && rhs.type() == V_BIGINT) {
    return Value(lhs.intValue() < rhs.intValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_BIGINT) {
    return Value(lhs.doubleValue() < static_cast<double>(rhs.intValue()));
  }
  else if (lhs.type() == V_BIGINT && rhs.type() == V_DOUBLE) {
    return Value(static_cast<double>(lhs.intValue()) < rhs.doubleValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_DOUBLE) {
    return Value(lhs.doubleValue() < rhs.doubleValue());
  }
  else {
    std::cerr << "error";
    exit(-1);
  }
}

Value greaterthan(const Value & lhs, const Value & rhs)
{
  if (lhs.type() == V_BIGINT && rhs.type() == V_BIGINT) {
    return Value(lhs.intValue() > rhs.intValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_BIGINT) {
    return Value(lhs.doubleValue() > static_cast<double>(rhs.intValue()));
  }
  else if (lhs.type() == V_BIGINT && rhs.type() == V_DOUBLE) {
    return Value(static_cast<double>(lhs.intValue()) > rhs.doubleValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_DOUBLE) {
    return Value(lhs.doubleValue() > rhs.doubleValue());
  }
  else {
    std::cerr << "error";
    exit(-1);
  }
}

Value lessorequal(const Value & lhs, const Value & rhs)
{
  if (lhs.type() == V_BIGINT && rhs.type() == V_BIGINT) {
    return Value(lhs.intValue() <= rhs.intValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_BIGINT) {
    return Value(lhs.doubleValue() <= static_cast<double>(rhs.intValue()));
  }
  else if (lhs.type() == V_BIGINT && rhs.type() == V_DOUBLE) {
    return Value(static_cast<double>(lhs.intValue()) <= rhs.doubleValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_DOUBLE) {
    return Value(lhs.doubleValue() <= rhs.doubleValue());
  }
  else {
    std::cerr << "error";
    exit(-1);
  }
}

Value greaterorequal(const Value & lhs, const Value & rhs)
{
  if (lhs.type() == V_BIGINT && rhs.type() == V_BIGINT) {
    return Value(lhs.intValue() >= rhs.intValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_BIGINT) {
    return Value(lhs.doubleValue() >= static_cast<double>(rhs.intValue()));
  }
  else if (lhs.type() == V_BIGINT && rhs.type() == V_DOUBLE) {
    return Value(static_cast<double>(lhs.intValue()) >= rhs.doubleValue());
  }
  else if (lhs.type() == V_DOUBLE && rhs.type() == V_DOUBLE) {
    return Value(lhs.doubleValue() >= rhs.doubleValue());
  }
  else {
    std::cerr << "error";
    exit(-1);
  }
}

Value comma(const Value & lhs, const Value & rhs)
{
  lhs;
  return rhs;
}
