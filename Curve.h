#pragma once

#include "FiniteField.h"
#include <sstream>
#include <iostream>
#include <cassert>

// Weierstrass curve
class Curve {
public:
    Curve(const Fp2& A, const Fp2& B, const Fp2& C): A_(A), B_(B), C_(C), A2_(A * A), C2_(C * C), C3_(C2_ * C) {
#ifdef DEBUG
        assert(cmp(A.get_x().get_p(), B.get_x().get_p()) == 0);
        assert(cmp(A.get_x().get_p(), C.get_x().get_p()) == 0);
        assert(!(A*A*A*Fp2(4, 0, A.get_x().get_p()) + C*B*B*Fp2(27, 0, B.get_x().get_p())).zero());
#endif
    }

    Curve(const Fp2& A, const Fp2& B): Curve(A, B, Fp2(1, 0, A.get_x().get_p())) {}

    Curve normalize() const {
        return Curve{A_/C_, B_/C_, Fp2(1, 0, C_.get_x().get_p())};
    }

    friend bool operator==(const Curve& a, const Curve& b) {
        return a.A_*b.C_ == b.A_*a.C_ && a.B_*b.C_ == b.B_*a.C_;
    }

    bool contain(Fp2 x, Fp2 y) {
        return y*y*C_ == x*x*x*C_ + A_ * x + B_;
    }

    Fp2 eval(Fp2 x) const {
        return x*x*x + A_/C_ * x + B_/C_;
    }

    Fp2 j_inv() const {
        auto A1 = A_ / C_, B1 = B_ / C_;
        A1 = A1*A1*A1*Fp2(4, 0, A_.get_x().get_p());
        B1 = B1*B1*Fp2(27, 0, A_.get_x().get_p());
        return Fp2(1728, 0, A_.get_x().get_p()) * (A1) / (A1+B1);
    }

    std::string str() const {
        return "y^2 = x^3 + " + (A_/C_).str() + "x + " + (B_/C_).str() + ", j(E) = " + j_inv().str();
    }

    Fp2 A() const { return A_; }
    Fp2 B() const { return B_; }
    Fp2 C() const { return C_; }
    Fp2 A2() const { return A2_; }
    Fp2 C2() const { return C2_; }
    Fp2 C3() const { return C3_; }
private:
    Fp2 A_, B_, C_;
    Fp2 A2_, C2_, C3_;
};

// rational point in Weierstrass curves, projective coordinate
class RPoint {
public:
    RPoint(Fp2 x, Fp2 y, Fp2 z, const Curve& c): x_(x), y_(y), z_(z), curve_(c) {
#ifdef DEBUG
        assert(cmp(x.get_x().get_p(), y.get_x().get_p()) == 0);
        assert(cmp(x.get_x().get_p(), z.get_x().get_p()) == 0);
        assert(verify());
#endif
    };

    RPoint(unsigned int x, unsigned int y, const Curve& c)
        : x_(Fp2(x, 0, c.A().get_x().get_p())), y_(Fp2(y, 0, c.A().get_x().get_p())), 
          z_(Fp2(1, 0, c.A().get_x().get_p())), curve_(c) {};

    RPoint normalize() const {
        return RPoint{x_/z_, y_/z_, Fp2(1, 0, z_.get_x().get_p()), curve_};
    }

    Fp2 x() const { return x_; }
    Fp2 y() const { return y_; }
    Fp2 z() const { return z_; }
    Curve curve() const { return curve_; }

    friend bool operator==(const RPoint& p, const RPoint& q);
    friend RPoint operator+(const RPoint& p, const RPoint& q);
    friend RPoint operator*(const RPoint& p, int k);

    RPoint& operator=(const RPoint& p) {
        x_ = p.x_; y_ = p.y_; z_ = p.z_;
        curve_ = p.curve_;
        return *this;
    }

    std::string str() const {
        std::ostringstream ss;
        Fp2 x = x_, y = y_, z = z_;
        if (z.zero()) return "O";
        else {
            x = x / z;
            y = y / z;
        }
        ss << "("  << x.get_x().get_v().str();
        if (!x.get_y().zero()) ss << " + " << x.get_y().get_v().str() << "i";
        ss << ", " << y.get_x().get_v().str();
        if (!y.get_y().zero()) ss << " + " << y.get_y().get_v().str() << "i";
        ss << ")";
        return ss.str();
    }

private:
    Fp2 x_, y_, z_;
    Curve curve_;

    bool verify() {
        Fp2 lhs = y_ * y_ * z_ * curve_.C();
        Fp2 rhs = x_ * x_ * x_ * curve_.C() + curve_.A() * x_ * z_ * z_ + curve_.B() * z_ * z_ * z_;
        return lhs == rhs;
    }
};

bool operator==(const RPoint& p, const RPoint& q);
RPoint operator+(const RPoint& p, const RPoint& q);
RPoint operator*(const RPoint& p, int k);
RPoint zero(const Curve& curve);
RPoint random_point(const Curve& curve);
int order(const RPoint& p);