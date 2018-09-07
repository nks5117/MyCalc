// token.cpp
// Copyright (c) 2018 Ni Kesu. All rights reserved.

#include <iostream>
#include <cctype>
#include <cstring>
#include "token.h"

using std::string;
using std::vector;

string removeBlank(const string & s)
{
  string r;
  for (auto c : s) {
    if (!isblank(c)) {
      r += c;
    }
  }
  return r;
}

Token getToken(const std::string & s, size_t i)
{
  Token r;
  if (isdigit(s[i])) {
    r.type = T_LITERAL;
    while (isdigit(s[i]) || s[i] == '.') {
      r.str += s[i];
      i++;
    }
    return r;
  }
  else if (isalpha(s[i]) || s[i] == '_') {
    r.type = T_IDENTIFIER;
    while (isalpha(s[i]) || isdigit(s[i]) || s[i] == '_') {
      r.str += s[i];
      i++;
    }
    return r;
  }
  else {
    size_t num = sizeof(operatorList) / sizeof(operatorList[0]);
    for (size_t j = 0; j < num; j++) {
      if (strlen(operatorList[j]) == 2) {
        if (s[i] == operatorList[j][0] && s[i+1] == operatorList[j][1]) {
          r.type = static_cast<TokenType>(j);
          r.str = operatorList[j];
          return r;
        }
      }
    }
    for (size_t j = 0; j < num; j++) {
      if (strlen(operatorList[j]) == 1) {
        if (s[i] == operatorList[j][0]) {
          r.type = static_cast<TokenType>(j);
          r.str = operatorList[j];
          return r;
        }
      }
    }
    std::cerr << "getToken Failed!";
    exit(-1);
  }
  
}

vector<Token> getTokenStream(const string & s)
{
  vector<Token> r;
  string t = removeBlank(s);
  for (size_t i = 0; i < t.size();) {
    r.push_back(getToken(t, i));
    i += r.back().str.size();
  }
  return r;
}
