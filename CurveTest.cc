#include "CurveTest.h"
#include "Curve.h"
#include <iostream>
using std::cout, std::endl;

void CurveTest() {
    printf("--------- Curve Test ---------\n");
    Curve c{Fp2(1, 0, 11), Fp2(0, 0, 11)};
    auto p = RPoint(1, 3, c);
    auto q = RPoint(1, 11-3, c);
    cout << "p = " << p.to_string() << endl;
    cout << "q = " << q.to_string() << endl;
    cout << "0 + p = " << (zero(c)+p).to_string() << endl;
    cout << "0 + q = " << (zero(c)+q).to_string() << endl;
    cout << "p + q = " << (p+q).to_string() << endl;
    for (int i = 1; i<=100; i++) {
        cout << "p * " << i << " = " << (p*i).to_string() << endl;
    }
    printf("--------- Curve Test Passed ---------\n");
}