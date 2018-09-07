// expression.cpp
// Copyright (c) 2018 Ni Kesu. All rights reserved.

#include "expression.h"
using std::vector;
using std::stack;

Value Expression::evaluate()
{
  vector<Token> tokenStr = infixToPostfix(getTokenStream(str_));
  stack<Value> s;
  for (auto iter = tokenStr.cbegin(); iter < tokenStr.cend(); ++iter) {
    if (isOperand(iter->type)) {
      auto t = toValue(*iter);
      s.push(t);
    }
    else if (isOperator(iter->type)) {
      int n = arity(iter->type);
      if (s.size() < n) {
        std::cerr << "too few operands";
        exit(-1);
      }
      if (n == 1) {
        Value t = s.top();
        s.pop();
        switch (iter->type) {
        case T_FACTORIAL:
          s.push(factorial(t));
          break;
        default:
          std::cerr << "undefined operator!";
          exit(-1);
          break;
        }
        
      }
      else if (n == 2) {
        Value r = s.top();
        s.pop();
        Value l = s.top();
        s.pop();
        switch (iter->type) {
        case T_POWER:
          s.push(power(l, r));
          break;
        case T_ASSIGN:
          s.push(assign(l, r));
          break;
        case T_PLUS:
          s.push(plus(l, r));
          break;
        case T_MINUS:
          s.push(minus(l, r));
          break;
        case T_MULTIPLY:
          s.push(multiply(l, r));
          break;
        case T_DIVIDE:
          s.push(divide(l, r));
          break;
        case T_MODULO:
          s.push(modulo(l, r));
          break;
        case T_GREATERTHAN:
          s.push(greaterthan(l, r));
          break;
        case T_LESSTHAN:
          s.push(lessthan(l, r));
          break;
        case T_GREATEROREQUAL:
          s.push(greaterorequal(l, r));
          break;
        case T_LESSOREQUAL:
          s.push(lessorequal(l, r));
          break;
        case T_EQUAL:
          s.push(equal(l, r));
          break;
        case T_NOTEQUAL:
          s.push(notequal(l, r));
          break;
        case T_COMMA:
          s.push(comma(l, r));
          break;
        default:
          std::cerr << "undefined operator!";
          exit(-1);
          break;
        }
        
      }
    }
  }
  if (s.size() == 1) {
    return s.top();
  }
  else {
    std::cerr << "too many operands";
    exit(-1);
  }
}


// use shunting-yard algorithm to convert a token stream from infix notation to 
// postfix notation
vector<Token> infixToPostfix(const vector<Token>& a)
{
  vector<Token> r;
  stack<Token> s;
  for (auto iter = a.cbegin(); iter < a.cend(); ++iter) {
    if (isOperand(iter->type)) {
      if (iter->type == T_IDENTIFIER && isFunction(iter->str)) {
        // TODO
      }
      r.push_back(*iter);
    }
    else if (iter->type == T_LEFTPAREN) {
      s.push(*iter);
    }
    else if (iter->type == T_RIGHTPAREN) {
      bool flag = false;
      while (!s.empty()) {
        if (s.top().type == T_LEFTPAREN) {
          s.pop();
          flag = true;
          break;
        }
        else {
          r.push_back(s.top());
          s.pop();
        }
      }
      if (!flag) {
        std::cerr << "missing parentheses";
        exit(-1);
      }
    }
    else { // isOperator(iter->type)
      while (!s.empty()
             && (precedence(s.top().type) > precedence(iter->type)
                 || (precedence(s.top().type) == precedence(iter->type)
                     && associative(s.top().type) == LEFT_ASSOCIATIVE)
                 || isFunction(s.top().str))) {
        r.push_back(s.top());
        s.pop();
      }
      s.push(*iter);
    }
  }
  while (!s.empty()) {
    if (s.top().type == T_LEFTPAREN || s.top().type == T_RIGHTPAREN) {
      std::cerr << "missing parentheses";
      exit(-1);
    }
    r.push_back(s.top());
    s.pop();
  }
  return r;
}
