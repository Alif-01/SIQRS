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
        Fp2 XX = p.x_.square();
        Fp2 ZZ = p.z_.square();
        Fp2 wC = p.curve_.A()*ZZ+p.curve_.C()*(XX+XX+XX);
        Fp2 s = p.y_ * p.z_; s = s+s;
        Fp2 ss = s.square();
        Fp2 sss = s*ss;
        Fp2 R = p.y_*s;
        Fp2 RR = R*R;
        Fp2 B = (p.x_+R).square()-XX-RR;
        Fp2 hC2 = wC.square()-(B+B)*p.curve_.C2();
        Fp2 X3 = hC2*s*p.curve_.C();
        Fp2 Y3 = wC*(B*p.curve_.C2()-hC2)-(RR+RR)*p.curve_.C3();
        Fp2 Z3 = sss*p.curve_.C3();
        return RPoint(X3, Y3, Z3, p.curve_);
    } else {
        const Fp2& X1=p.x_, Y1=p.y_, Z1=p.z_;
        const Fp2& X2=q.x_, Y2=q.y_, Z2=q.z_;
        Fp2 Y1Z2 = Y1*Z2;
        Fp2 X1Z2 = X1*Z2;
        Fp2 Z1Z2 = Z1*Z2;
        Fp2 u = Y2*Z1 - Y1Z2;
        Fp2 uu = u.square();
        Fp2 v = X2*Z1 - X1Z2;
        Fp2 vv = v.square();
        Fp2 vvv = v*vv;
        Fp2 R = vv*X1Z2;
        Fp2 A = uu*Z1Z2 - vvv - R - R;
        Fp2 X3 = v*A;
        Fp2 Y3 = u*(R-A) - vvv*Y1Z2;
        Fp2 Z3 = vvv*Z1Z2;
        return RPoint(X3, Y3, Z3, p.curve_);
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

int order(const RPoint& p) {
    RPoint s = zero(p.curve());
    for (int i = 1;; i++) {
        s = s + p;
        if (s == zero(p.curve())) return i;
    }
}