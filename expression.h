// expression.h
// Copyright (c) 2018 Ni Kesu. All rights reserved.

#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__
#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include "bigint.h"
#include "token.h"
#include "value.h"
#include "operators.h"
class Expression {
private:
  std::string str_;
public:
  Expression(const std::string &str) :str_(str) {};
  ~Expression() {};
  Value evaluate();
};

std::vector<Token> infixToPostfix(const std::vector<Token> &a);

#endif