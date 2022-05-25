// created by zjt on 

#pragma once

#include "FiniteField.h"
#include <sstream>
#include <iostream>
#include <cassert>

// Weierstrass curve
class Curve {
public:
    Curve(const Fp2& A, const Fp2& B): A_(A), B_(B) {
#ifdef DEBUG
        assert(cmp(A.get_x().get_p(), B.get_x().get_p()) == 0);
        assert(!(A*A*A*Fp2(4, 0, A.get_x().get_p()) + B*B*Fp2(27, 0, B.get_x().get_p())).zero());
#endif
    }

    friend bool operator==(const Curve& a, const Curve& b) {
        return a.A_ == b.A_ && a.B_ == b.B_;
    }

    bool contain(Fp2 x, Fp2 y) {
        return y*y == x*x*x + A_ * x + B_;
    }

    Fp2 A() const { return A_; }
    Fp2 B() const { return B_; }
private:
    Fp2 A_, B_;
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

    Fp2 x() const { return x_; }
    Fp2 y() const { return y_; }
    Fp2 z() const { return z_; }
    Curve curve() const { return curve_; }

    friend bool operator==(const RPoint& p, const RPoint& q);
    friend RPoint operator+(const RPoint& p, const RPoint& q);
    friend RPoint operator*(const RPoint& p, int k);

    RPoint& operator=(const RPoint& p) {
#ifdef DEBUG
        assert(curve_ == p.curve_);
#endif
        x_ = p.x_; y_ = p.y_; z_ = p.z_;
        return *this;
    }

    std::string to_string() const {
        std::ostringstream ss;
        Fp2 x = x_, y = y_, z = z_;
        if (z.zero()) return "O";
        else {
            x = x / z;
            y = y / z;
        }
        ss << "("  << x.get_x().get_v().to_string();
        if (!x.get_y().zero()) ss << " + " << x.get_y().get_v().to_string() << "i";
        ss << ", " << y.get_x().get_v().to_string();
        if (!y.get_y().zero()) ss << " + " << y.get_y().get_v().to_string() << "i";
        ss << ")";
        return ss.str();
    }

private:
    Fp2 x_, y_, z_;
    Curve curve_;

    bool verify() {
        Fp2 lhs = y_ * y_ * z_;
        Fp2 rhs = x_ * x_ * x_ + curve_.A() * x_ * z_ * z_ + curve_.B() * z_ * z_ * z_;
        return lhs == rhs;
    }
};

bool operator==(const RPoint& p, const RPoint& q);
RPoint operator+(const RPoint& p, const RPoint& q);
RPoint operator*(const RPoint& p, int k);
RPoint zero(const Curve& curve);
RPoint random_point(const Curve& curve);