#include "IsogenyTest.h"
#include "Isogeny.h"
#include "Parameters.h"
using std::cout, std::endl;
using std::vector;

vector<RPoint> get_rpoints(const Curve& c) {
    vector<RPoint> rpoints{zero(c)};
    auto P = c.A().get_x().get_p();
    for (unsigned int xa = 0; cmp(xa, P); xa++)
        for (unsigned int xb = 0; cmp(xb, P); xb++) {
            Fp2 x{xa, xb, P}, z{1, 0, P};
            auto y = c.eval(x).sqrt();
            if(y.first){
                rpoints.emplace_back(x, y.second, z, c);
                if(!(y.second == Fp2(0, 0, P)))
                    rpoints.emplace_back(x, -y.second, z, c);
            }
        }
    return rpoints;
}

void test_isogeny(const Curve& c, const RPoint& g, const vector<RPoint>& pts) {
    cout << "Now testing isogeny: {" + c.str()+"} / <"+g.str()+">" << endl;
    Isogeny phi(c, g);
    Curve d = phi.e1();
    cout << "image curve: " << d.str() << endl;
    assert(phi.map(g) == zero(d));
    assert(phi.map(g+g) == zero(d));
    cout << "kernel correct" << endl;
    int k = 100;
    while(k--) {
        auto p = pts[rand()%pts.size()], q = pts[rand()%pts.size()];
        assert(phi.map(p+q) == phi.map(p)+phi.map(q));
    }
    cout << "homomorphism correct" << endl;
}

void test_isogeny_chain(const Curve& c, const RPoint& g, const vector<RPoint>& pts, int l, int k) {
    cout << "Now testing isogeny chain: {" + c.str()+"} / <"+g.str()+">" << ", |g|=" << l << "^" << k << endl;
    IsogenyChain chain(c, g, l, k);
    auto d = chain.get_curve(k);
    cout << "image curve: " << d.str() << endl;
    assert(chain.map(g) == zero(d));
    cout << "kernel correct" << endl;
    int K = 10;
    while(K--) {
        auto p = pts[rand()%pts.size()], q = pts[rand()%pts.size()];
        assert(chain.map(p+q) == chain.map(p)+chain.map(q));
    }
    cout << "homomorphism correct" << endl;
}

void IsogenyTest() {
    printf("--------- Isogeny Test ---------\n");
    auto P = 23;
    cout << "Isogeny test on p = " << P << endl;
    Fp2 d = PRNG::prng.random_fp2(P);
    Curve c{Fp2(1, 0, P)*d, Fp2(0, 0, P)*d, d};
    cout << "E: " << c.str() << endl;
    auto rpoints = get_rpoints(c);
    cout << "#E[Fp2] = " << rpoints.size() << endl;
    for(auto p : rpoints) {
        int o = order(p);
        if (o == 2) {
            test_isogeny_chain(c, p, rpoints, 2, 1);
        }
        if (o == 4) {
            test_isogeny_chain(c, p, rpoints, 2, 2);
        }
        if (o == 8) {
            test_isogeny_chain(c, p, rpoints, 2, 3);
        }
        if (o == 3) {
            test_isogeny_chain(c, p, rpoints, 3, 1);
        }
    }
    printf("--------- Isogeny Test Passed ---------\n");
}