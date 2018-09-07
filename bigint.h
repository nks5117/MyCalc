// bigint.h
// Copyright (c) 2018 Ni Kesu. All rights reserved.

#ifndef __BIGINT_H__
#define __BIGINT_H__
#include <cstdint>
#include <string>
#include <vector>
#include <cstring>

const int SHIFT = 30; // use base 2^30
typedef uint16_t halfDigit;
typedef uint32_t digit;
typedef uint64_t twoDigits;
typedef int16_t signedHalfDigit;
typedef int32_t signedDigit;
typedef int64_t signedTwoDigit;
const digit MASK = 0x3fffffff;
const digit MAX_DIGIT = MASK;
const digit BASE = 0x40000000;
class BigInt
{
private:
  digit * digits_; // pointer to array of digits, nullptr for zero
  size_t size_; // digits in array, 0 for zero
  int sign_; // 0 for positive or zero, 1 for negative number

  void removeLeadingZeros();
public:
  std::string toString() const;
  void fromString(const std::string &s);
  
  BigInt(const BigInt &a);
  BigInt(const digit *digits, size_t size, int sign);
  BigInt(const std::string &s);
  explicit BigInt(digit num);
  explicit BigInt(signedHalfDigit num = 0);
  explicit BigInt(signedDigit num);
  explicit BigInt(signedTwoDigit num);
  explicit BigInt(double num);
  explicit operator signedHalfDigit() const;
  explicit operator signedDigit() const;
  explicit operator signedTwoDigit() const;
  explicit operator twoDigits() const;
  explicit operator double() const;
  BigInt &operator=(const BigInt &a);
  ~BigInt();
//friends
  friend BigInt operator-(const BigInt &l);
  friend BigInt operator+(const BigInt &l, const BigInt &r);
  friend BigInt operator-(const BigInt &l, const BigInt &r);
  friend BigInt operator*(const BigInt &l, const BigInt &r);
  friend BigInt operator/(const BigInt &l, const BigInt &r);
  friend BigInt operator%(const BigInt &l, const BigInt &r);
  friend bool operator==(const BigInt &l, const BigInt &r);
  friend bool operator>(const BigInt &l, const BigInt &r);
  friend bool operator<(const BigInt &l, const BigInt &r);
  friend bool operator>=(const BigInt &l, const BigInt &r);
  friend bool operator<=(const BigInt &l, const BigInt &r);
  friend bool operator!=(const BigInt &l, const BigInt &r);
  friend BigInt abs(const BigInt &l);
  friend digit oneDigitQuotientDivision(const BigInt & a,
                                        const BigInt & b,
                                        BigInt *pr);
};

const BigInt ZERO(static_cast<int16_t>(0)),
ONE(static_cast<int16_t>(1)),
TWO(static_cast<int16_t>(2)),
TEN(static_cast<int16_t>(10)),
HUNDRED(static_cast<int16_t>(100)),
THOUSAND(static_cast<int16_t>(1000));
const BigInt NEG_ONE = -ONE;


inline BigInt &operator+=(BigInt &l, const BigInt &r) {
  return l = l + r;
}
inline BigInt &operator-=(BigInt &l, const BigInt &r) {
  return l = l - r;
}
inline BigInt &operator*=(BigInt &l, const BigInt &r) {
  return l = l * r;
}
inline BigInt &operator/=(BigInt &l, const BigInt &r) {
  return l = l / r;
}
inline BigInt &operator++(BigInt &r) {
  return r = r + ONE;
}
inline BigInt &operator--(BigInt &r) {
  return r = r - ONE;
}

BigInt pow(const BigInt &l, const BigInt &r);
BigInt pow(const BigInt &l, int r);


digit fullAdder(digit n1, digit n2, digit carryIn, digit *carryOut);
digit fullSubtractor(digit n1, digit n2, digit borrowIn, digit *borrowOut);
BigInt powerOfBase(long long int n); // calculate base^n
BigInt polynomialEvaluate(const std::vector<int> &a, int n);
bool isMersennePrime(int p);
#endif // !__BIGINT_H__