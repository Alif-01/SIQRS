#include "Curve.h"

bool operator==(const RPoint& p, const RPoint& q) {
    return p.x_*q.z_ == q.x_*p.z_ && p.y_*q.z_ == q.y_*p.z_;
}

RPoint operator+(const RPoint& p, const RPoint& q) {
#ifdef DEBUG
    assert(p.curve_ == q.curve_);
#endif
    if (p.z_.zero()) return q;
    if (q.z_.zero()) return p;
    if (p == q) {
        Fp2 _8{8, 0, p.x_.get_x().get_p()};
        Fp2 _12{12, 0, p.x_.get_x().get_p()};
        Fp2 yz = p.y_ * p.z_; Fp2 y2z = p.y_ * yz; Fp2 xy2z = p.x_ * y2z;
        Fp2 x2 = p.x_ * p.x_;
        Fp2 tx = p.z_ * p.z_ + x2 + x2 + x2; Fp2 tx2 = tx * tx;
        Fp2 A = p.curve_.A(); Fp2 A2 = A * A;
        Fp2 x3 = (yz + yz) * (A2 * tx2 - xy2z * _8);
        Fp2 y3 = A * tx * (xy2z * _12 - A2 * tx2) - y2z * y2z * _8;
        Fp2 z3 = yz * yz * yz * _8; // 19M
        return RPoint(x3, y3, z3, p.curve_);
    } else {
        Fp2 x2z1 = q.x_ * p.z_; Fp2 x1z2 = p.x_ * q.z_;
        Fp2 y2z1 = q.y_ * p.z_; Fp2 y1z2 = p.y_ * q.z_;
        Fp2 z1z2 = p.z_ * q.z_;
        Fp2 tx = x2z1 - x1z2; Fp2 tx2 = tx * tx; Fp2 tx3 = tx2 * tx;
        Fp2 ty = y2z1 - y1z2; Fp2 ty2 = ty * ty;
        Fp2 x3 = tx * (ty2 * z1z2 - tx2 * (x2z1 + x1z2));
        Fp2 y3 = ty * (tx2 * (x2z1 + x1z2 + x1z2) - ty2 * z1z2) - tx3 * y1z2;
        Fp2 z3 = tx3 * z1z2; // 16M
        return RPoint(x3, y3, z3, p.curve_);
    }
}

RPoint operator*(const RPoint& p, int k) {
    RPoint s = zero(p.curve_), t = p;
    while(k) {
        if (k&1) s = s + t;
        k >>= 1;
        t = t + t;
    }
    return s;
}

RPoint zero(const Curve& curve) {
    auto p = curve.A().get_x().get_p();
    return RPoint{Fp2(0, 0, p), Fp2(1, 0, p), Fp2(0, 0, p), curve};
}

RPoint random_point(const Curve& curve) {
    while(1) {
    }
}