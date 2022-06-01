#pragma once
#include "Curve.h"
#include <algorithm>
using std::cout, std::endl, std::vector;

// Isogeny of degree 2 or 3
class Isogeny {
public:
    // isogeny E -> E/<g>
    // |g| = 2 or 3
    Isogeny(const Curve& e0, RPoint g): e0_(e0), e1_(e0), g_(g) {
#ifdef DEBUG
        assert(g.curve() == e0);
#endif
        auto p0 = zero(e0), g2 = g + g;
        auto P = g.x().get_x().get_p();
        assert(!(g==p0) && (g2 == p0 || g2+g == p0));
        z1 = g.z() * e0.C();
        F a(e0.A()*g.z(), 1, 0), b(e0.B()*g.z(), 1, 0);

        xp = F(e0.C()*g.x(), 1, 0);
        yp = F(e0.C()*g.y(), 1, 0);
        gxp = mul(xp, xp);
        gxp = add(add(gxp, gxp), add(gxp, a));
        gyp = F(Fp2(0, 0, P)-yp.v-yp.v, 1, 0);
        up = mul(gyp, gyp);
        if (g2 == p0) {
            vp = gxp;
        } else {
            vp = add(gxp, gxp);
        }
        gxpgyp = mul(gxp, gyp);

        F v(vp);
        F w(add(up,mul(xp,vp)));
        F a1 = add(a, mul(v, Fp2(P-5, 0, P)));
        F b1 = add(b, mul(w, Fp2(P-7, 0, P)));

        Fp2 A1, B1, C1;
        set_projective(A1, B1, C1, a1, b1, F(Fp2(1, 0, P), 0, 0));
        e1_ = Curve(A1, B1, C1);
    }

    RPoint map(RPoint p) {
#ifdef DEBUG
        assert(p.curve() == e0_);
#endif
        if (p == zero(p.curve())) return zero(e1_);

        z2 = p.z();
        F x(p.x(), 0, 1), y(p.y(), 0, 1);
        F dx = sub(x, xp), dx2 = mul(dx, dx), dx3 = mul(dx2, dx);
        F t1 = mul(up, y), t2 = sub(mul(vp, sub(y, yp)), gxpgyp);
        F x1 = add(mul(dx3, x), add(mul(dx2, vp), mul(dx, up)));
        F y1 = sub(mul(dx3, y), add(add(t1, t1), mul(dx, t2)));
        F z1 = dx3;
        Fp2 X, Y, Z;
        set_projective(X, Y, Z, x1, y1, z1);
        return RPoint{X, Y, Z, e1_};
    }

    Curve e1() { return e1_; }

private:
    struct F {
        Fp2 v;
        int d1, d2;

        F() {}
        F(Fp2 v, int d1=0, int d2=0): v(v), d1(d1), d2(d2) {}

        std::string str() const {
            std::string s = v.str();
            if(d1) s += "/z1^" + std::to_string(d1);
            if(d2) s += "/z2^" + std::to_string(d2);
            return s;
        }
    };

    Curve e0_, e1_;
    RPoint g_;
    Fp2 z1, z2;
    F gxp, gyp, gxpgyp, vp, up, xp, yp;

    Fp2 expand(F f, int d1, int d2) const {
        auto v = f.v;
        for(int i=f.d1; i<d1; i++) v = v * z1;
        for(int i=f.d2; i<d2; i++) v = v * z2;
        return v;
    }

    F mul(const F& x, const Fp2& y) {
        return F(x.v*y, x.d1, x.d2);
    }

    F mul(const F& x, const F& y) {
        return F(x.v*y.v, x.d1+y.d1, x.d2+y.d2);
    }

    F add(const F& x, const F& y) {
        int d1 = std::max(x.d1, y.d1), d2 = std::max(x.d2, y.d2);
        return F(expand(x, d1, d2)+expand(y, d1, d2), d1, d2);
    }

    F sub(const F& x, const F& y) {
        int d1 = std::max(x.d1, y.d1), d2 = std::max(x.d2, y.d2);
        return F(expand(x, d1, d2)-expand(y, d1, d2), d1, d2);
    }

    void set_projective(Fp2& x, Fp2& y, Fp2& z, const F& a, const F& b, const F& c) {
        int d1 = std::max(std::max(a.d1, b.d1), c.d1);
        int d2 = std::max(std::max(a.d2, b.d2), c.d2);
        x = expand(a, d1, d2);
        y = expand(b, d1, d2);
        z = expand(c, d1, d2);
    }
};

class IsogenyChain {
public:
    IsogenyChain(const Curve& c, const RPoint& g, int dl, int dk): dl(dl), dk(dk) {
#ifdef DEBUG
        assert(dl==2 || dl==3);
        RPoint tmp = g;
        for (int i=0; i<dk-1; i++) {
            if (dl==2) tmp = tmp+tmp;
            else tmp = tmp+tmp+tmp;
        }
        assert(!(tmp == zero(c)));
        if (dl==2) tmp = tmp+tmp;
        else tmp = tmp+tmp+tmp;
        assert(tmp == zero(c));
#endif

        plan_partition(1.0, 1.0, 1.0);
        curves.push_back(c);
        dfs(0, 0, g);
        assert((int)curves.size() == dk+1);
        assert((int)isogenies.size() == dk);
    }

    RPoint map(const RPoint& p) {
        auto q = p;
        for(auto f : isogenies) {
            q = f.map(q);
        }
        return q;
    }

    int get_dl() const { return dl; }
    int get_dk() const { return dk; }

    Curve get_curve(int k) const {
        assert(k>=0 && k<=dk);
        return curves[k];
    }

private:
    vector<Curve> curves;
    vector<Isogeny> isogenies;
    vector<int> partition;
    int dl, dk;

    void plan_partition(double cl, double cr, double c1) {
        vector<double> cost{0, c1};
        partition = vector<int>{0, 0};
        for (int i=2; i<=dk; i++) {
            cost.push_back(1e50);
            partition.push_back(-1);
            for (int j=1; j<i; j++) {
                if (cost[j] + cost[i-j] + cl*(i-j) + cr*j < cost[i]) {
                    cost[i] = cost[j] + cost[i-j] + cl*(i-j) + cr*j;
                    partition[i] = j;
                }
            }
        }
    }

    // [l^(k-j)]R_j at E_j
    void dfs(int k, int j, RPoint p) {
        int sz = dk-k, ls = partition[sz], rs = sz-ls;
        if(sz == 1) {
            isogenies.emplace_back(curves[j], p);
            curves.push_back(isogenies[j].e1());
            return;
        }
        RPoint pl = p, pr = p;
        for (int i=0; i<sz-ls; i++) {
            if (dl==2) pl = pl + pl;
            else pl = pl + pl + pl;
        }
        dfs(k+sz-ls, j, pl);
        for (int i=0; i<sz-rs; i++) {
            pr = isogenies[j+i].map(pr);
        }
        dfs(k+sz-rs, j+sz-rs, pr);
    }
};
