// main.cpp
// Copyright (c) 2018 Ni Kesu. All rights reserved.

#ifndef __OPERATORS_H__
#define __OPERATORS_H__
#include "value.h"

Value factorial(const Value &n);

Value power(const Value &lhs, const Value &rhs);
Value assign(Value &lhs, const Value &rhs);
Value plus(const Value &lhs, const Value &rhs);
Value minus(const Value &lhs, const Value &rhs);
Value multiply(const Value &lhs, const Value &rhs);
Value divide(const Value &lhs, const Value &rhs);
Value modulo(const Value &lhs, const Value &rhs);

Value equal(const Value &lhs, const Value &rhs);
Value notequal(const Value &lhs, const Value &rhs);
Value lessthan(const Value &lhs, const Value &rhs);
Value greaterthan(const Value &lhs, const Value &rhs);
Value lessorequal(const Value &lhs, const Value &rhs);
Value greaterorequal(const Value &lhs, const Value &rhs);

Value comma(const Value &lhs, const Value &rhs);
#endif