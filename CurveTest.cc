#include "CurveTest.h"
#include "Curve.h"
#include <iostream>
#include <vector>
using std::cout, std::endl;
using std::vector;

int order(RPoint p) {
    for (int i = 1;; i++) {
        if (p * i == zero(p.curve())) return i;
    }
}

void group_test(unsigned int P) {
    assert(P%4 == 3);
    cout << "EC group test on p = " << P << endl;
    Curve c{Fp2(1, 0, P), Fp2(0, 0, P)};
    cout << "E: y^2 = x^3 + x" << endl;
    vector<RPoint> rpoints{zero(c)};
    for (unsigned int xa = 0; xa < P; xa++)
        for (unsigned int xb = 0; xb < P; xb++)
            for (unsigned int ya = 0; ya < P; ya++)
                for (unsigned int yb = 0; yb < P; yb++) {
                    Fp2 x{xa, xb, P}, y{ya, yb, P}, z{1, 0, P};
                    if(c.contain(x, y)) rpoints.emplace_back(x, y, z, c);
                }
    cout << "#E[Fp2] = " << rpoints.size() << endl;
    for (auto p : rpoints) {
        assert(rpoints.size() % order(p) == 0);
    }
    cout << "element order test passed" << endl;
}

void CurveTest() {
    printf("--------- Curve Test ---------\n");
    group_test(11);
    printf("--------- Curve Test Passed ---------\n");
}