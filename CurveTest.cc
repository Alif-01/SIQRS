#include "CurveTest.h"
#include "Curve.h"
#include <iostream>
#include <vector>
using std::cout, std::endl;
using std::vector;

void group_test(unsigned int P) {
    cout << "EC group test on p = " << P << endl;
    Curve c{PRNG::prng.random_fp2(P), PRNG::prng.random_fp2(P), PRNG::prng.random_fp2(P)};
    cout << "E: " << c.str() << endl;
    vector<RPoint> rpoints{zero(c)};
    for (unsigned int xa = 0; xa < P; xa++)
        for (unsigned int xb = 0; xb < P; xb++) {
            Fp2 x{xa, xb, P}, z{1, 0, P};
            auto y = c.eval(x).sqrt();
            if(y.first){
                rpoints.emplace_back(x, y.second, z, c);
                if(!(y.second == Fp2(0, 0, P)))
                    rpoints.emplace_back(x, -y.second, z, c);
            }
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