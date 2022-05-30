#pragma once

#include "gmp.h"
#include <utility>
#include <cassert>
#include <string>
#include <cstdio>

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
  friend bool operator==(const Integer &a, const Integer &b);
  friend Integer inv(const Integer &a, const Integer &b);
  friend Integer pow(const Integer &base, unsigned int exp);
  friend Integer powm(const Integer &base, const Integer &exp, const Integer &mod);
  friend int cmp(const Integer &a, const Integer &b);
  friend class Fp;
  friend class Fp2;
  friend class PRNG;
  friend Fp operator-(const Fp &a);

  std::string to_string() const {
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
bool operator==(const Integer &a, const Integer &b);
Integer inv(const Integer &a, const Integer &b);
Integer pow(const Integer &base, unsigned int exp);
Integer powm(const Integer &base, const Integer &exp, const Integer &mod);
int cmp(const Integer &a, const Integer &b);

/**
 * Usage: random_number = PRNG::prng.random(p);
 */
class PRNG {
 public:
  static PRNG prng;
  /**
   * Generate a uniform random integer in the range 0 to n − 1, inclusive.
   * @param n
   * @return
   */
  Integer random(const Integer &n) {
    Integer res;
    mpz_urandomm(res.v_, state_, n.v_);
    return res;
  }
 private:
  PRNG() {
    gmp_randinit_default(state_);
    gmp_randseed_ui(state_, 998244353);
  }
  gmp_randstate_t state_;
};

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

  std::pair<bool, Fp> sqrt() const {
    int legendre = mpz_legendre(this->v_.v_, this->p_.v_);
    if (legendre == 1) {
      Fp res = *this;
      Integer exp;
      mpz_cdiv_q_ui(exp.v_, this->p_.v_, 4u); // exp = (p+1)/4 = ceil(p/4)
      mpz_powm(res.v_.v_, this->v_.v_, exp.v_, this->p_.v_);
      return std::make_pair(true, res);
    } else {
      return std::make_pair(false, *this);
    }
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

  friend Fp2 operator-(const Fp2 &a);
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

  std::pair<bool, Fp2> sqrt() const {
    const Fp &x = x_;
    const Fp &y = y_;
    const Integer &p = x_.p_;
    const Fp zero = Fp(0, p);

    if (y == zero) {
      int legendre = mpz_legendre(x.v_.v_, p.v_);
      if (legendre == 1) {
        auto pair = x.sqrt();
        return std::make_pair(true, Fp2(pair.second, zero));
      } else {
        auto pair = (-x).sqrt();
        return std::make_pair(true, Fp2(zero, pair.second));
      }
    }

    // y != zero
    const Fp &a = y;
    const Fp b = x / y;
    const Fp c = b * b + Fp(1, p);
    int legendre = mpz_legendre(c.v_.v_, p.v_);
    if (legendre == -1) {
      return std::make_pair(false, *this);
    }
    auto pair_u = c.sqrt();
    Fp u = pair_u.second;
    Fp inverse2a = Fp(2, p).inv() * a;
    int pm = mpz_legendre((inverse2a * (b + u).inv()).v_.v_, p.v_);
    Fp bu = (pm == 1) ? (b + u) : (b - u);
    Fp2 bu2 = Fp2(bu, zero);
    auto pair_v = (inverse2a * bu.inv()).sqrt();
    Fp2 v = Fp2(pair_v.second, zero);
    return std::make_pair(true, (bu2 + Fp2(0, 1, p)) * v);
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

Fp2 operator-(const Fp2 &a);
Fp2 operator+(const Fp2 &a, const Fp2 &b);
Fp2 operator-(const Fp2 &a, const Fp2 &b);
Fp2 operator*(const Fp2 &a, const Fp2 &b);
Fp2 operator*(const Fp2 &a, const unsigned int &b);
Fp2 operator/(const Fp2 &a, const Fp2 &b);
bool operator==(const Fp2 &a, const Fp2 &b);