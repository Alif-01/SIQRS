#include "FiniteField.h"

// Integer

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

bool operator==(const Integer &a, const Integer &b) {
  return cmp(a, b) == 0;
}

Integer inv(const Integer &a, const Integer &b) {
  Integer res;
  mpz_invert(res.v_, a.v_, b.v_);
  return res;
}

Integer pow(const Integer &base, unsigned int exp) {
  Integer res;
  mpz_pow_ui(res.v_, base.v_, exp);
  return res;
}

Integer powm(const Integer &base, const Integer &exp, const Integer &mod) {
  Integer res;
  mpz_powm(res.v_, base.v_, exp.v_, mod.v_);
  return res;
}

int cmp(const Integer &a, const Integer &b) {
  return mpz_cmp(a.v_, b.v_);
}

// PRNG

PRNG PRNG::prng;

// Fp

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

// Fp2

Fp2 operator-(const Fp2 &a) {
  return Fp2{-a.x_, -a.y_};
}

Fp2 operator+(const Fp2 &a, const Fp2 &b) {
  return Fp2{a.x_ + b.x_, a.y_ + b.y_};
}

Fp2 operator-(const Fp2 &a, const Fp2 &b) {
  return Fp2{a.x_ - b.x_, a.y_ - b.y_};
}

Fp2 operator*(const Fp2 &a, const Fp2 &b) {
  Fp c2 = a.x_ * b.x_;
  Fp c0 = a.y_ * b.y_;
  Fp c1 = (a.x_ + a.y_) * (b.x_ + b.y_) - c2 - c0;
  return Fp2{c2 - c0, c1};
} // 3M

Fp2 operator*(const Fp2 &a, const unsigned int &b) {
  return Fp2{a.x_ * b, a.y_ * b};
}

Fp2 operator/(const Fp2 &a, const Fp2 &b) {
  return a * b.inv();
}

bool operator==(const Fp2 &a, const Fp2 &b) {
  return a.x_ == b.x_ && a.y_ == b.y_;
}