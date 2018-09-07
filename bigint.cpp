// bigint.cpp
// Copyright (c) 2018 Ni Kesu. All rights reserved.

#include "bigint.h"
#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <algorithm>
#ifdef _DEBUG
#include <cassert>
#include <ctime>
#endif
using std::string;
using std::vector;
void BigInt::removeLeadingZeros()
{
  size_t newSize = size_;
  while (newSize != 0) {
    if (digits_[newSize - 1] == 0) {
      newSize--;
    }
    else {
      break;
    }
  }
  size_ = newSize;
  if (size_ != 0) {
    digit *newDigits = new digit[newSize];
    memcpy(newDigits, digits_, newSize * sizeof(digit));
    delete[] digits_;
    digits_ = newDigits;
  }
  else {
    delete[] digits_;
    digits_ = nullptr;
  }
}

string BigInt::toString() const
{
  if (*this == ZERO) {
    return "0";
  }
  string str;
  auto cpy = abs(*this);
  while (cpy > ZERO) {
    auto tem = cpy / TEN;
    str += static_cast<char>('0' + static_cast<int>(cpy - tem * TEN));
    //std::cout << str.size() << std::endl;
    cpy = tem;
  }
  if (this->sign_ == 1) {
    str += '-';
  }
  return string(str.rbegin(), str.rend());
}

void BigInt::fromString(const string &s)
{
  errno = 0;
  long num = strtol(s.c_str(), nullptr, 10);
  if (errno != ERANGE) {
    delete[] digits_;
    digits_ = nullptr;
    *this = BigInt(static_cast<signedTwoDigit>(num));
    return;
  }
  
  vector<int> a;
  for (auto iter = s.crbegin(); iter!=s.crend(); ++iter) {
    a.push_back(*iter - '0');
  }
  if (a.back() + '0' == '-' - '0' + '0') {
    a.pop_back();
  }
  delete[] digits_;
  digits_ = nullptr;
  *this = polynomialEvaluate(a, 10);
  return;
}

BigInt::BigInt(const BigInt & a) : size_(a.size_), sign_(a.sign_)
{
  if (size_ == 0) {
    digits_ = nullptr;
    return;
  }
  digits_ = new digit[size_];
  memcpy(digits_, a.digits_, size_ * sizeof(digit));
}

BigInt::BigInt(const digit * digits, size_t size, int sign) :
  size_(size),
  sign_(sign)
{
  if (size == 0) {
    digits_ = nullptr;
  }
  else {
    this->digits_ = new digit[size];
    memcpy(digits_, digits, size * sizeof(digit));
  }
  removeLeadingZeros();
}

BigInt::BigInt(const std::string & s)
{
  digits_ = nullptr;
  fromString(s);
}

BigInt::BigInt(digit num)
{
  this->digits_ = nullptr;
  *this = BigInt(static_cast<signedTwoDigit>(num));
}

BigInt::BigInt(signedHalfDigit num)
{
  sign_ = 0;
  if (num == 0) {
    digits_ = nullptr;
    size_ = 0;
  }
  else {
    size_ = 1;
    if (num < 0) {
      sign_ = 1;
      num = -num;
    }
    digits_ = new digit[size_];
    digits_[0] = num & MASK;
  }
}

BigInt::BigInt(signedDigit num)
{
  sign_ = 0;
  if (num == 0) {
    digits_ = nullptr;
    size_ = 0;
  }
  else {
    size_ = 1;
    if (num < 0) {
      sign_ = 1;
      num = -num;
    }
    if (num >> SHIFT) {
      size_ = 2;
    }
    digits_ = new digit[size_];
    digits_[0] = num & MASK;
    if (size_ == 2) {
      digits_[1] = (num >> SHIFT);
    }
  }
}

BigInt::BigInt(signedTwoDigit num)
{
  sign_ = 0;
  if (num == 0) {
    digits_ = nullptr;
    size_ = 0;
  }
  else {
    if (num < 0) {
      sign_ = 1;
      num = -num;
    }
    size_ = 1;
    if (num >> SHIFT) {
      size_ = 2;
    }
    if (num >> SHIFT >> SHIFT) {
      size_ = 3;
    }
    digits_ = new digit[size_];
    for (size_t i = 0; i < size_; i++) {
      digits_[i] = (num >> (SHIFT*i))&MASK;
    }
  }
}

BigInt::BigInt(double num)
{
  // TODO
  *this = static_cast<BigInt>(static_cast<signedTwoDigit>(num));
}

BigInt::operator signedHalfDigit() const
{
  return size_ == 0 ? 0 : 
    static_cast<signedHalfDigit>(sign_ == 1 ?
                                 -static_cast<signedDigit>(digits_[0])
                                 : digits_[0]);
}

BigInt::operator signedDigit() const
{
  if (size_ == 0) {
    return 0;
  }
  else if (size_ == 1) {
    return sign_ == 1 ? -static_cast<signedDigit>(digits_[0]) : digits_[0];
  }
  else {
    return sign_ == 1 ? 
      -static_cast<signedDigit>(digits_[0] | (digits_[1] << SHIFT))
      : (digits_[0] | (digits_[1] << SHIFT));
  }
}

BigInt::operator signedTwoDigit() const
{
  if (size_ == 0) {
    return 0;
  }
  signedTwoDigit ret = 0;
  for (size_t i = 0; i < size_; i++) {
    ret |= static_cast<signedTwoDigit>(digits_[i]) << (SHIFT*i);
  }
  if (sign_ == 1) {
    ret = -ret;
  }
  return ret;
}

BigInt::operator twoDigits() const
{
  return static_cast<twoDigits>(static_cast<signedTwoDigit>(*this));
}

BigInt::operator double() const
{
  return strtod(toString().c_str(), nullptr);
}



BigInt & BigInt::operator=(const BigInt & a)
{
  if (this == &a) {
    return *this;
  }
  delete[] digits_;
  digits_ = nullptr;
  size_ = a.size_;
  sign_ = a.sign_;
  digits_ = new digit[size_];
  memcpy(digits_, a.digits_, size_*sizeof(digit));
  return *this;
}

BigInt::~BigInt()
{
  delete[] digits_;
}

BigInt operator-(const BigInt & l)
{
  BigInt ret = l;
  ret.sign_ = (ret.sign_ == 0 ? 1 : 0);
  return ret;
}

BigInt operator+(const BigInt & l, const BigInt & r)
{
  if (l.size_ == 0) {
    return r;
  }
  if (r.size_ == 0) {
    return l;
  }
  if (l.sign_ != r.sign_) {
    return l - (-r);
  }
  if (l.size_ < r.size_) {
    return r + l;
  }

  size_t size = l.size_ + 1;
  digit *digits = new digit[size];
  memset(digits, 0, (l.size_ + 1) * sizeof(digit));
  int sign = l.sign_;

  for (size_t i = 0; i < r.size_; i++) {
    digits[i] = fullAdder(l.digits_[i], r.digits_[i], 
                          digits[i], &digits[i + 1]);
  }

  for (size_t i = r.size_; i < l.size_; i++) {
    digits[i] = fullAdder(l.digits_[i], static_cast<digit>(0),
                          digits[i], &digits[i + 1]);
  }
  BigInt res(digits, size, sign);
  delete[] digits;
  return res;
}

BigInt operator-(const BigInt & l, const BigInt & r)
{
  if (l.size_ == 0) {
    return -r;
  }
  if (r.size_ == 0) {
    return l;
  }
  if (l.sign_ != r.sign_) {
    return l + (-r);
  }
  if (abs(l) == abs(r)) {
    return ZERO;
  }
  if (abs(l) < abs(r)) {
    return -(r - l);
  }

  size_t size = l.size_ + 1;
  digit *digits = new digit[size];
  memset(digits, 0, (l.size_ + 1) * sizeof(digit));
  int sign = l.sign_;

  for (size_t i = 0; i < r.size_; i++) {
    digits[i] = fullSubtractor(l.digits_[i], r.digits_[i],
                               digits[i], &digits[i + 1]);
  }

  for (size_t i = r.size_; i < l.size_; i++) {
    digits[i] = fullSubtractor(l.digits_[i], static_cast<digit>(0),
                               digits[i], &digits[i + 1]);
  }

  
  BigInt res(digits, size, sign);
  delete[] digits;
  return res;
}

BigInt operator*(const BigInt & l, const BigInt & r)
{
  if (l == ZERO || r == ZERO) {
    return ZERO;
  }
  if (l == ONE) {
    return r;
  }
  if (r == ONE) {
    return l;
  }
  if (l == NEG_ONE) {
    return -r;
  }
  if (r == NEG_ONE) {
    return -l;
  }
  

  if (r == TWO) {
    size_t size = l.size_ + 1;
    int sign = l.sign_;
    digit *digits = new digit[size];
    for (size_t i = 1; i <= size; i++) {
      if (i == 1) {
        digits[size - i] = l.digits_[size - i - 1] << 1 >> SHIFT
                         & static_cast<digit>(1);
      }
      else if (i == size) {
        digits[size - i] = (l.digits_[size - i] << 1 & MASK);
      }
      else {
        digits[size - i] = (l.digits_[size - i] << 1 & MASK) |
          (l.digits_[size - i - 1] << 1 >> SHIFT & static_cast<digit>(1));
      }
    }
    BigInt res(digits, size, sign);
    delete[] digits;
    return res;
  }


  BigInt res = ZERO;
  int sign = l.sign_ == r.sign_ ? 0 : 1;
  for (size_t i = 0; i < r.size_; i++) {
    size_t size = l.size_ + 1 + i;
    digit *digits = new digit[size];
    memset(digits, 0, (size) * sizeof(digit));
    for (size_t j = 0; j < l.size_; j++) {
      twoDigits tem = static_cast<twoDigits>(r.digits_[i])
                    * l.digits_[j] + digits[i + j];
      digits[i + j] = tem & MASK;
      digits[i + j + 1] += (tem >> SHIFT) & MASK;
#ifdef _DEBUG
      assert(tem >> SHIFT >> SHIFT == 0);
#endif
    }
    //res += BigInt(digits, size, sign);
    res = res + BigInt(digits, size, sign);
    delete[] digits;
  }

  return res;
}

BigInt operator/(const BigInt & l, const BigInt & r)
{
  if (r == ZERO) {
    std::cerr << "divide by zero!";
    exit(-1);
  }
  if (l == ZERO || l < r) {
    return ZERO;
  }
  if (r == ONE) {
    return l;
  }
  if (r == NEG_ONE) {
    return -l;
  }

  if (l.size_ == 1 && r.size_ == 1) {
    if (l.sign_ == r.sign_) {
      return BigInt(l.digits_[0] / r.digits_[0]);
    }
    else {
      return BigInt(-static_cast<signedDigit>(l.digits_[0] / r.digits_[0]));
    }
  }

  if (r == TWO) {
    BigInt res = l;
    for (size_t i = 0; i < res.size_; i++) {
      res.digits_[i] >>= 1;
      if (i + 1 < res.size_) {
        res.digits_[i] |= res.digits_[i + 1] & static_cast<digit>(1);
      }
    }
    res.removeLeadingZeros();
    return res;
  }

  BigInt a = abs(l), b = abs(r);
  size_t qSize = a.size_ - b.size_ + 1;
  digit *q = new digit[qSize];
  memset(q, 0, qSize * sizeof(digit));
  int sign = l.sign_ == r.sign_ ? 0 : 1;
  
  BigInt tem = ZERO;
  if (b.size_ > 1) {
    digit *temDigits = new digit[b.size_ - 1];
    for (int i = 0; i < b.size_ - 1; i++) {
      temDigits[b.size_ - 2 - i] = a.digits_[a.size_ - 1 - i];
    }
    tem = BigInt(temDigits, b.size_ - 1, 0);
    delete[]temDigits;
  }
  
  for (size_t i = 1; i <= qSize; i++) {
    q[qSize - i] = oneDigitQuotientDivision(tem*BigInt(BASE) +
                                            BigInt(a.digits_[qSize - i]),
                                            b, &tem);
  }

  
  BigInt res(q, qSize, sign);
#ifdef _DEBUG
  if (a - res * b < ZERO) {
    std::cout << "!!!!!!!\n";
  }
  assert(a - res * b >= ZERO);
  assert(a - res * b < b);
#endif
  delete[]q;
  return res;
  /*
  BigInt res = ZERO, n = abs(l), d = abs(r), step = ONE;
  while (1) {
    if (n - step * d >= ZERO) {
      n = n - step * d;
      res = res + ONE * step;
      step = step * TWO;
    }
    else {
      if (step == ONE) {
        break;
      }
      else {
        step /= TWO;
      }
    }
  }
  if (l.sign_ != r.sign_) {
    res = -res;
  }
  return res;
  */

}

BigInt operator%(const BigInt & l, const BigInt & r)
{
  return l - l / r * r;
}

bool operator==(const BigInt & l, const BigInt & r)
{
  if (l.sign_ != r.sign_) {
    return false;
  }
  if (l.size_ != r.size_) {
    return false;
  }
  for (size_t i = 0; i < l.size_; i++) {
    if (l.digits_[i] != r.digits_[i]) {
      return false;
    }
  }
  return true;
}

bool operator>(const BigInt & l, const BigInt & r)
{
  if (l.sign_ == 0 && r.sign_ == 1) {
    return true;
  }
  else if (l.sign_ == 1 && r.sign_ == 0) {
    return false;
  }
  else if (l.sign_ == 1 && r.sign_ == 1) {
    if (l.size_ > r.size_) {
      return false;
    }
    else if (l.size_ < r.size_) {
      return true;
    }
    else { // l.size_ == r.size_
      for (size_t i = 1; i <= l.size_; i++) {
        if (l.digits_[l.size_ - i] > r.digits_[l.size_ - i]) {
          return false;
        }
        if (l.digits_[l.size_ - i] < r.digits_[l.size_ - i]) {
          return true;
        }
      }
      return false;
    }
  }
  else { // l.sign_ == 0 && r.sign_ == 0
    if (l.size_ > r.size_) {
      return true;
    }
    else if (l.size_ < r.size_) {
      return false;
    }
    else { // l.size_ == r.size_
      for (size_t i = 1; i <= l.size_; i++) {
        if (l.digits_[l.size_ - i] > r.digits_[l.size_ - i]) {
          return true;
        }
        if (l.digits_[l.size_ - i] < r.digits_[l.size_ - i]) {
          return false;
        }
      }
      return false;
    }
  }
}

bool operator<(const BigInt & l, const BigInt & r)
{
  return (!(l == r)) && (!(l > r));
}

bool operator>=(const BigInt & l, const BigInt & r)
{
  return l > r || l == r;
}

bool operator<=(const BigInt & l, const BigInt & r)
{
  return l < r || l == r;
}

bool operator!=(const BigInt & l, const BigInt & r)
{
  return !(l == r);
}

BigInt abs(const BigInt & l)
{
  return BigInt(l.digits_, l.size_, 0);
}

digit oneDigitQuotientDivision(const BigInt & a,
                               const BigInt & b,
                               BigInt *pr)
{
  if (a < b) {
    *pr = a;
    return static_cast<digit>(0);
  }
  if (a.size_ == 1 && b.size_ == 1) {
    digit q = a.digits_[0] / b.digits_[0];
    if (pr != nullptr) {
      *pr = a - BigInt(q) * b;
    }
    return q;
  }
  else if (b.digits_[b.size_ - 1] < BASE / 2) {
    digit q = oneDigitQuotientDivision(a*BigInt(BASE/(b.digits_[b.size_-1]+1)),
                                       b*BigInt(BASE/(b.digits_[b.size_-1]+1)),
                                       nullptr);
    if (pr != nullptr) {
      *pr = a - BigInt(q) * b;
    }
    return q;
  }
  else {
    twoDigits t1;
    if (a.size_ > b.size_) {
      t1 = static_cast<twoDigits>(a.digits_[a.size_ - 1]) * BASE +
        a.digits_[a.size_ - 2];
    }
    else if (a.size_ == b.size_) {
      t1 = a.digits_[a.size_ - 1];
    }
    else {
      std::cerr << "Fail in oneDigitQuotientDivision()\n";
      exit(-1);
    }
    twoDigits t2 = b.digits_[b.size_ - 1];
    digit q = static_cast<digit>(std::min(static_cast<twoDigits>(BASE - 1),
                                          t1 / t2));
    q = q - 2;
    BigInt r = a - static_cast<BigInt>(q)*b;
    while (r >= b) {
      q++;
      r -= b;
    }
    if (pr != nullptr) {
      *pr = r;
    }
    return q;
  }
}

BigInt pow(const BigInt & l, const BigInt & r)
{
  return pow(l, static_cast<int>(r));
}

BigInt pow(const BigInt & l, int r)
{
#ifdef _DEBUG
  auto startTime = clock();
#endif // _DEBUG

  
  BigInt res = ONE;
  for (int i = 0; i < r; i++) {
    res *= l;
  }

#ifdef _DEBUG
  auto endTime = clock();
  std::cout << "pow() use "
    << static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC
    << "s" << std::endl;
#endif // _DEBUG

  return res;
}

digit fullAdder(digit n1, digit n2, digit carryIn, digit *carryOut)
{
  digit s = n1 + n2 + carryIn;
  *carryOut = s >> SHIFT;
  s &= MASK;
  return s;
}

digit fullSubtractor(digit n1, digit n2, digit borrowIn, digit *borrowOut)
{
  digit s = n1 - n2 - borrowIn;
  *borrowOut = (s >> SHIFT == 0 ? 0 : 1);
  s &= MASK;
  return s;
}

BigInt powerOfBase(long long int n)
{
  size_t size = n+1;
  int sign = 0;
  digit *digits = new digit[size];
  memset(digits, 0, size * sizeof(digit));
  digits[n] = 1;
  BigInt res(digits, size, sign);
  delete[] digits;
  return res;
}

// use Qin Jiushao's Algorithm
BigInt polynomialEvaluate(const std::vector<int>& a, int n)
{
  BigInt r, x(n);
  for (auto iter = a.crbegin(); iter != a.crend(); ++iter) {
    r = BigInt(*iter) + x * r;
  }
  return r;
}

// use Lucas-Lehmer primality test to determine whether Mp is prime.
bool isMersennePrime(int p)
{
  BigInt mp = pow(TWO, p) - ONE;
  BigInt s(4);
  for (int i = 0; i < p - 2; i++) {

    //std::cout << i << std::endl;

    if ((s = (((s*s) - TWO) % mp)) == ZERO) {
      return true;
    }
  }
  return false;
}

