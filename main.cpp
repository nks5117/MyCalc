// main.cpp
// Copyright (c) 2018 Ni Kesu. All rights reserved.

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "bigint.h"
#include "token.h"
#include "expression.h"
#include "variable.h"

#ifdef TIME
#include <ctime>
#endif


using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::string;

#define PROMPT '$'

std::vector<Variable> variableList;

void title() {
  cout << "MyCalc v1.5.0\n";
  cout << "[Type \"exit\" to exit]\n";
}

int main()
{
  title();
  variableList.push_back(Variable("ans", V_NA, ZERO, 0.0));
  variableList.push_back(Variable("pi", V_DOUBLE, ZERO, 3.1415926535897));
  variableList.push_back(Variable("e", V_DOUBLE, ZERO, 2.7182818284590));
  variableList.push_back(Variable("c", V_BIGINT, BigInt(299792458), 0.0));
  cout.precision(10);
  Value ans;

  while (1) {
    cout << PROMPT << ' ';
    string s;
    getline(cin, s);
    if (s == "exit") {
      break;
    }
    bool showAnswer = (s[s.size() - 1] != ';');
    if (!showAnswer) {
      s.erase(s.end()-1);
    }

#ifdef TIME
    auto startTime = clock();
#endif

    ans = Expression(s).evaluate();
    if (showAnswer) {
      cout << "ans =\n\n      ";
    }
    if (ans.type() == V_DOUBLE) {
      if (showAnswer) {
        cout << ans.doubleValue() << endl << endl;
      }
      *(variableList[0].doubleValue()) = ans.doubleValue();
    }
    else if(ans.type() == V_BIGINT) {
      if (showAnswer) {
        cout << ans.intValue().toString() << endl << endl;

#ifdef TIME
        auto endTime = clock();
        cout << "total use " << (double)(endTime - startTime) / CLOCKS_PER_SEC
          << 's' << endl << endl;
#endif

      }
      *(variableList[0].intValue()) = ans.intValue();
    }
    variableList[0].setType(ans.type());

  }

  return 0;
}


