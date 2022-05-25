#pragma once

#include "gmp.h"
#include <utility>
#include <cassert>
#include <string>

class Fp;
class Fp2;

class Integer {
 public:
  Integer(unsigned int x = 0) {
    mpz_init_set_ui(v_, x);
  }
  Integer(std::string str) {
    mpz_init_set_str(v_, str.c_str(), 10);
  }
  Integer(const Integer &integer) {
    mpz_init_set(v_, integer.v_);
  }
  Integer(Integer &&integer) {
    mpz_init(v_);
    mpz_swap(v_, integer.v_);
  }
  ~Integer() {
    mpz_clear(v_);
  }

  Integer &operator =(const Integer &integer) {
    mpz_set(v_, integer.v_);
    return *this;
  }
  Integer &operator =(Integer &&integer) {
    mpz_swap(v_, integer.v_);
    return *this;
  }
  friend Integer operator+(const Integer &a, const Integer &b);
  friend Integer operator-(const Integer &a, const Integer &b);
  friend Integer operator*(const Integer &a, const Integer &b);
  friend Integer operator*(const Integer &a, const unsigned int &b);
  friend Integer operator%(const Integer &a, const Integer &b);
  friend Integer inv(const Integer &a, const Integer &b);
  friend int cmp(const Integer &a, const Integer &b);
  friend class Fp;
  friend Fp operator-(const Fp &a);

  std::string to_string() {
    return std::string{mpz_get_str(NULL, 10, v_)};
  }

 private:
  mpz_t v_;
};

Integer operator+(const Integer &a, const Integer &b);
Integer operator-(const Integer &a, const Integer &b);
Integer operator*(const Integer &a, const Integer &b);
Integer operator*(const Integer &a, const unsigned int &b);
Integer operator%(const Integer &a, const Integer &b);
Integer inv(const Integer &a, const Integer &b);
int cmp(const Integer &a, const Integer &b);

class Fp {
 public:
  Fp(Integer v, Integer p): v_{v}, p_{p} {
#ifdef DEBUG
    assert(cmp(p%4, 3) == 0);
#endif
  }
  ~Fp() = default;

  friend Fp operator-(const Fp &a);
  friend Fp operator+(const Fp &a, const Fp &b);
  friend Fp operator-(const Fp &a, const Fp &b);
  friend Fp operator*(const Fp &a, const Fp &b);
  friend Fp operator*(const Fp &a, const unsigned int &b);
  friend Fp operator/(const Fp &a, const Fp &b);
  friend bool operator==(const Fp &a, const Fp &b);
  friend class Fp2;

  Fp inv() const{
    return Fp{::inv(v_, p_), p_};
  }

  Fp sqrt() const {
    return *this;
  }

  Integer get_v() const {
    return v_;
  }

  Integer get_p() const {
    return p_;
  }

  std::string to_string() {
    return v_.to_string() + " ( mod " + p_.to_string() + " )";
  }

  bool zero() const{
    return cmp(v_, 0) == 0;
  }

 private:
  Integer v_;
  Integer p_;
};

Fp operator-(const Fp &a);
Fp operator+(const Fp &a, const Fp &b);
Fp operator-(const Fp &a, const Fp &b);
Fp operator*(const Fp &a, const Fp &b);
Fp operator*(const Fp &a, const unsigned int &b);
Fp operator/(const Fp &a, const Fp &b);
bool operator==(const Fp &a, const Fp &b);

// scalar in F_{p^2}
// x + iy
class Fp2 {
 public:
  Fp2(Integer x, Integer y, Integer p) : x_{x, p}, y_{y, p} {}
  Fp2(Fp x, Fp y) : x_{x}, y_{y} {
#ifdef DEBUG
    assert(cmp(x.p_, y.p_) == 0);
#endif
  }
  ~Fp2() = default;

  friend Fp2 operator+(const Fp2 &a, const Fp2 &b);
  friend Fp2 operator-(const Fp2 &a, const Fp2 &b);
  friend Fp2 operator*(const Fp2 &a, const Fp2 &b); // 3M
  friend Fp2 operator*(const Fp2 &a, const unsigned int &b);
  friend Fp2 operator/(const Fp2 &a, const Fp2 &b);
  friend bool operator==(const Fp2 &a, const Fp2 &b);

  // 2M
  Fp2 square() const {
    Fp x = (x_ + y_) * (x_ - y_);
    Fp y = x_ * y_ * 2;
    return Fp2{x, y};
  }

  // 1I + 2S + 2M
  Fp2 inv() const {
    Fp denominator = x_ * x_ + y_ * y_;
    Fp denominator_inv = denominator.inv();
    Fp x = x_ * denominator_inv;
    Fp y = -y_ * denominator_inv;
    return Fp2{x, y};
  }

  Fp get_x() const {
    return x_;
  }

  Fp get_y() const {
    return y_;
  }

  std::string to_string() const {
    return x_.get_v().to_string() + " + " + y_.get_v().to_string() + " i ( mod " + x_.get_p().to_string() + " )";
  }

  bool zero() const {
    return x_.zero() && y_.zero();
  }

 private:
  Fp x_;
  Fp y_;
};

Fp2 operator+(const Fp2 &a, const Fp2 &b);
Fp2 operator-(const Fp2 &a, const Fp2 &b);
Fp2 operator*(const Fp2 &a, const Fp2 &b);
Fp2 operator*(const Fp2 &a, const unsigned int &b);
Fp2 operator/(const Fp2 &a, const Fp2 &b);
bool operator==(const Fp2 &a, const Fp2 &b);