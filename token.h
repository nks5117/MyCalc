// token.h
// Copyright (c) 2018 Ni Kesu. All rights reserved.

#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>
#include <vector>

enum TokenType {
  T_NULL, T_LEFTPAREN, T_RIGHTPAREN,
  T_PLUS, T_MINUS, T_MULTIPLY, T_DIVIDE, T_MODULO, T_POWER, T_FACTORIAL,
  T_EQUAL, T_NOTEQUAL, T_LESSTHAN, T_GREATERTHAN,
  T_LESSOREQUAL, T_GREATEROREQUAL,
  T_ASSIGN, T_COMMA, 
  T_LITERAL = 50, T_IDENTIFIER
};

enum AssociativeType{
  A_UNDEFINED, LEFT_ASSOCIATIVE, RIGHT_ASSOCIATIVE
};

struct Token {
  std::string str;
  TokenType type;
};

const char * const operatorList[] = {
  "", "(", ")",
  "+", "-", "*", "/", "%", "^", "!",
  "==", "!=", "<", ">",
  "<=", ">=",
  "=", ","
};

inline bool isOperator(TokenType n) {
  return n < 50;
}
inline bool isOperand(TokenType n) {
  return n >= 50;
}
inline int precedence(TokenType n) {
  switch (n) {
  case T_FACTORIAL:
    return 100;
  case T_POWER:
    return 88;
  case T_MULTIPLY: case T_DIVIDE: case T_MODULO:
    return 56;
  case T_PLUS: case T_MINUS:
    return 39;
  case T_GREATERTHAN: case T_LESSTHAN:
  case T_GREATEROREQUAL: case T_LESSOREQUAL:
    return 27;
  case T_EQUAL: case T_NOTEQUAL:
    return 12;
  case T_ASSIGN:
    return 5;
  case T_COMMA:
    return 1;
  default:
    return -1;
  }
}
inline AssociativeType associative(TokenType n) {
  switch (n) {
  case T_POWER: case T_ASSIGN:
    return RIGHT_ASSOCIATIVE;
  case T_FACTORIAL: case T_PLUS: case T_MINUS:
  case T_MULTIPLY: case T_DIVIDE: case T_MODULO:
  case T_GREATERTHAN: case T_LESSTHAN:
  case T_GREATEROREQUAL: case T_LESSOREQUAL:
  case T_EQUAL: case T_NOTEQUAL:
  case T_COMMA:
    return LEFT_ASSOCIATIVE;
  default:
    return A_UNDEFINED;
  }
}
inline int arity(TokenType n) {
  switch (n) {
  case T_POWER: case T_ASSIGN:
  case T_PLUS: case T_MINUS:
  case T_MULTIPLY: case T_DIVIDE: case T_MODULO:
  case T_GREATERTHAN: case T_LESSTHAN:
  case T_GREATEROREQUAL: case T_LESSOREQUAL:
  case T_EQUAL: case T_NOTEQUAL:
  case T_COMMA:
    return 2;
  case T_FACTORIAL:
    return 1;
  default:
    return -1;
  }

}

inline bool isFunction(std::string s) {
  // TODO
  return false;
}
std::string removeBlank(const std::string &s);
Token getToken(const std::string &s, size_t i);
std::vector<Token> getTokenStream(const std::string &s);

#endif