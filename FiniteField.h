#pragma once
#define DEBUG

#include "gmp.h"
#include <utility>
#include <cassert>

class Integer {
 public:
  Integer(unsigned int x = 0) {
    mpz_init_set_ui(v_, x);
  }
  Integer(const char *str) {
    mpz_init_set_str(v_, str, 10);
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
  friend Integer operator/(const Integer &a, const Integer &b);
  friend Integer operator%(const Integer &a, const Integer &b);
  friend std::pair<Integer, Integer> div_qr(const Integer &a, const Integer &b);
  friend Integer inv(const Integer &a, const Integer &b);
  friend int cmp(const Integer &a, const Integer &b);

  char *to_string() {
    return mpz_get_str(NULL, 10, v_);
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
Integer operator/(const Integer &a, const Integer &b) {
  Integer q;
  mpz_cdiv_q(q.v_, a.v_, b.v_);
  return q;
}
Integer operator%(const Integer &a, const Integer &b) {
  Integer r;
  mpz_cdiv_r(r.v_, a.v_, b.v_);
  return r;
}
/**
 * Usage: auto [q, r] = div_qr(a, b), where q = a / b, r = a % b
 * @param a
 * @param b
 * @return
 */
std::pair<Integer, Integer> div_qr(const Integer &a, const Integer &b) {
  Integer q, r;
  mpz_cdiv_qr(q.v_, r.v_, a.v_, b.v_);
  return std::make_pair(q, r);
}
Integer inv(const Integer &a, const Integer &b) {
  Integer res;
  mpz_invert(res.v_, a.v_, b.v_);
  return res;
}
int cmp(const Integer &a, const Integer &b) {
  return mpz_cmp(a.v_, b.v_);
}

class Fp2;

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
  friend Fp operator/(const Fp &a, const Fp &b);
  friend class Fp2;

  Fp inv() const{
    return Fp{::inv(v_, p_), p_};
  }
  char *to_string() {
    return v_.to_string();
  }
 private:
  Integer v_;
  Integer p_;
};

Fp operator-(const Fp &a) {
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
  if (cmp(c, b.v_) >= 0) {
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
Fp operator/(const Fp &a, const Fp &b) {
#ifdef DEBUG
  assert(cmp(a.p_, b.p_) == 0);
#endif
  Integer c = a.v_ * b.inv().v_ % a.p_;
  return Fp{c, a.p_};
}

// scalar in F_{p^2}
// x + iy
class Fp2 {
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
  friend Fp2 operator/(const Fp2 &a, const Fp2 &b);

  // 2M
  Fp2 square() const {
    Fp x = (x_ + y_) * (x_ - y_);
    Fp y = Fp{2, x_.p_} * x_ * y_;
    return Fp2{x, y};
  }
  // 1I + 2S + 2M
  Fp2 inv() const {
    Fp deno = x_ * x_ + y_ * y_;
    Fp x = x_ * deno;
    Fp y = -y_ * deno;
    return Fp2{x, y};
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
Fp2 operator/(const Fp2 &a, const Fp2 &b) {
  return a * b.inv();
}
