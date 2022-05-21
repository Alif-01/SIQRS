#pragma once
#define DEBUG

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

Integer operator+(const Integer &a, const Integer &b) {
  Integer c;
  mpz_add(c.v_, a.v_, b.v_);
  return c;
}
Integer operator-(const Integer &a, const Integer &b) {
  Integer c;
  mpz_sub(c.v_, a.v_, b.v_);
  return c;
}
Integer operator*(const Integer &a, const Integer &b) {
  Integer c;
  mpz_mul(c.v_, a.v_, b.v_);
  return c;
}
Integer operator*(const Integer &a, const unsigned int &b) {
  Integer c;
  mpz_mul_ui(c.v_, a.v_, b);
  return c;
}
Integer operator%(const Integer &a, const Integer &b) {
  Integer r;
  mpz_mod(r.v_, a.v_, b.v_);
  return r;
}
Integer inv(const Integer &a, const Integer &b) {
  Integer res;
  mpz_invert(res.v_, a.v_, b.v_);
  return res;
}
int cmp(const Integer &a, const Integer &b) {
  return mpz_cmp(a.v_, b.v_);
}

class Fp {
 public:
  Fp(Integer x, Integer p): v_{std::move(x)}, p_{std::move(p)} {}
  Fp(const Fp &fp): v_{fp.v_}, p_{fp.p_} {}
  Fp(Fp &&fp): v_{fp.v_}, p_{fp.p_} {}
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
  Integer get_v() const {
    return v_;
  }
  Integer get_p() const {
    return p_;
  }
  std::string to_string() {
    return v_.to_string() + " ( mod " + p_.to_string() + " )";
  }

 private:
  Integer v_;
  Integer p_;
};

Fp operator-(const Fp &a) {
  if (mpz_sgn(a.v_.v_) == 0) {
    return a;
  }
  return Fp{a.p_, a.p_} - a;
}
Fp operator+(const Fp &a, const Fp &b) {
#ifdef DEBUG
  assert(cmp(a.p_, b.p_) == 0);
#endif
  Integer c = a.v_ + b.v_;
  if (cmp(c, a.p_) >= 0) {
    c = c - a.p_;
  }
  return Fp{c, a.p_};
}
Fp operator-(const Fp &a, const Fp &b) {
#ifdef DEBUG
  assert(cmp(a.p_, b.p_) == 0);
#endif
  Integer c = a.v_;
  if (cmp(c, b.v_) < 0) {
    c = c + a.p_;
  }
  c = c - b.v_;
  return Fp{c, a.p_};
}
Fp operator*(const Fp &a, const Fp &b) {
#ifdef DEBUG
  assert(cmp(a.p_, b.p_) == 0);
#endif
  Integer c = a.v_ * b.v_ % a.p_;
  return Fp{c, a.p_};
}
Fp operator*(const Fp &a, const unsigned &b) {
  Integer c = a.v_ * b % a.p_;
  return Fp{c, a.p_};
}
Fp operator/(const Fp &a, const Fp &b) {
#ifdef DEBUG
  assert(cmp(a.p_, b.p_) == 0);
#endif
  Integer c = a.v_ * b.inv().v_ % a.p_;
  return Fp{c, a.p_};
}
bool operator==(const Fp &a, const Fp &b) {
  return cmp(a.p_, b.p_) == 0 && cmp(a.v_, b.v_) == 0;
}

// scalar in F_{p^2}
// x + iy
class Fp2 {
 public:
  Fp2(Integer x, Integer y, Integer p) : x_{std::move(x), p}, y_{std::move(y), p} {}
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

 private:
  Fp x_;
  Fp y_;
};

Fp2 operator+(const Fp2 &a, const Fp2 &b) {
  return Fp2{a.x_ + b.x_, a.y_ + b.y_};
}
Fp2 operator-(const Fp2 &a, const Fp2 &b) {
  return Fp2{a.x_ - b.x_, a.y_ - b.y_};
}
// 3M
Fp2 operator*(const Fp2 &a, const Fp2 &b) {
  Fp c2 = a.x_ * b.x_;
  Fp c0 = a.y_ * b.y_;
  Fp c1 = (a.x_ + a.y_) * (b.x_ + b.y_) - c2 - c0;
  return Fp2{c2 - c0, c1};
}
Fp2 operator*(const Fp2 &a, const unsigned int &b) {
  return Fp2{a.x_ * b, a.y_ * b};
}
Fp2 operator/(const Fp2 &a, const Fp2 &b) {
  return a * b.inv();
}
bool operator==(const Fp2 &a, const Fp2 &b) {
  return a.x_ == b.x_ && a.y_ == b.y_;
}