#include "Protocol.h"
#include "FiniteField.h"
#include <chrono>

using std::cout, std::endl;

void run_protocol() {
  auto start1 = std::chrono::steady_clock::now();

    Integer P = SIDH_P;
    Curve E(Fp2(1, 0, P), Fp2(0, 0, P), Fp2(1, 0, P));
    RPoint Pa = random_point(E).pow3(K3), Qa = random_point(E).pow3(K3);
    RPoint Pb = random_point(E).pow2(K2), Qb = random_point(E).pow2(K2);
    while(Pa.pow2(K2-1) == zero(E)) Pa = random_point(E).pow3(K3);
    while(Qa.pow2(K2-1) == zero(E)) Qa = random_point(E).pow3(K3);
    while(Pb.pow3(K3-1) == zero(E)) Pb = random_point(E).pow2(K2);
    while(Qb.pow3(K3-1) == zero(E)) Qb = random_point(E).pow2(K2);

  auto start2 = std::chrono::steady_clock::now();

    // Alice's secrets
    Integer na = PRNG::prng.random(P2); 
    RPoint pa = Pa + Qa * na;
    IsogenyChain iso_a(E, pa, 2, K2);

  auto start3 = std::chrono::steady_clock::now();

    // Bob's secrets
    Integer nb = PRNG::prng.random(P3); 
    RPoint pb = Pb + Qb * nb;
    IsogenyChain iso_b(E, pb, 3, K3);

  auto start4 = std::chrono::steady_clock::now();

    // exchange messages
    Curve Ea = iso_a.e1();
    RPoint Pba = iso_a.map(Pb), Qba = iso_a.map(Qb);
    Curve Eb = iso_b.e1();
    RPoint Pab = iso_b.map(Pa), Qab = iso_b.map(Qa);

  auto start5 = std::chrono::steady_clock::now();

    // Alice's shared key
    RPoint pa2 = Pab + Qab * na;
    IsogenyChain iso_a2(Eb, pa2, 2, K2);
    Fp2 key_a = iso_a2.e1().j_inv();

  auto start6 = std::chrono::steady_clock::now();

    // Bob's shared key
    RPoint pb2 = Pba + Qba * nb;
    IsogenyChain iso_b2(Ea, pb2, 3, K3);
    Fp2 key_b = iso_b2.e1().j_inv();

  auto start7 = std::chrono::steady_clock::now();

    cout << endl << "-------------------------" << endl << endl;
    cout << "Alice's secrets:" << endl << endl;
    cout << "  na = " << na.str() << endl << endl;
    cout << "  isogeny: E / <" << pa.str() << ">" << endl;

    cout << endl << "-------------------------" << endl << endl;
    cout << "Bob's secrets:" << endl << endl;
    cout << "  nb = " << nb.str() << endl << endl;
    cout << "  isogeny: E / <" << pb.str() << ">" << endl;

    cout << endl << "-------------------------" << endl << endl;
    cout << "Alice's key: " << key_a.str() << endl << endl;
    cout << "Bob's key: " << key_b.str() << endl << endl;

    if (key_a == key_b) cout << "Key exchange successful!" << endl;
    else cout << "Key exchange failed..." << endl;

  std::chrono::duration<double> elapsed_seconds1 = start2-start1;
  std::chrono::duration<double> elapsed_seconds2 = start3-start2;
  std::chrono::duration<double> elapsed_seconds3 = start4-start3;
  std::chrono::duration<double> elapsed_seconds4 = start5-start4;
  std::chrono::duration<double> elapsed_seconds5 = start6-start5;
  std::chrono::duration<double> elapsed_seconds6 = start7-start6;
    cout << "Time elapsed 1: " << elapsed_seconds1.count() << "s" << endl;
  cout << "Time elapsed 2: " << elapsed_seconds2.count() << "s" << endl;
  cout << "Time elapsed 3: " << elapsed_seconds3.count() << "s" << endl;
  cout << "Time elapsed 4: " << elapsed_seconds4.count() << "s" << endl;
  cout << "Time elapsed 5: " << elapsed_seconds5.count() << "s" << endl;
  cout << "Time elapsed 6: " << elapsed_seconds6.count() << "s" << endl;
}