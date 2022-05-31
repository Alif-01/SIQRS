//
// Created by yww on 2022/5/21.
//

#include "FiniteFieldTest.h"
#include "FiniteField.h"
#include "Parameters.h"

void FiniteFieldTest() {
  printf("--------- Finite Field Test ---------\n");
  Integer p(11); // 4k + 3
  printf("p = %s\n", p.to_string().c_str());

  // Fp
  Fp a(3, p);
  Fp b(4, p);
  Fp sum_a_b(7, p);
  Fp diff_a_b(10, p);
  Fp prod_a_b(1, p);
  Fp quot_a_b(9, p);
  Fp sqrt_a(5, p);
  assert(a + b == sum_a_b);
  assert(a - b == diff_a_b);
  assert(a * b == prod_a_b);
  assert(a / b == quot_a_b);
  auto pair_a = a.sqrt();
  assert(pair_a.first && (pair_a.second == sqrt_a || -pair_a.second == sqrt_a));
  printf("a = %s\n", a.to_string().c_str());
  printf("b = %s\n", b.to_string().c_str());
  printf("a + b = %s\n", (a + b).to_string().c_str());
  printf("a - b = %s\n", (a - b).to_string().c_str());
  printf("a * b = %s\n", (a * b).to_string().c_str());
  printf("a / b = %s\n", (a / b).to_string().c_str());
  printf("sqrt(a) = %s\n", pair_a.second.to_string().c_str());

  // Fp2
  Fp2 c(1, 2, p);
  Fp2 d(3, 4, p);
  Fp2 sum_c_d(4, 6, p);
  Fp2 diff_c_d(9, 9, p);
  Fp2 prod_c_d(6, 10, p);
  Fp2 quot_c_d(0, 8, p);
  Fp2 sqrt_c(2, 6, p);
  assert(c + d == sum_c_d);
  assert(c - d == diff_c_d);
  assert(c * d == prod_c_d);
  assert(c / d == quot_c_d);
  auto pair_c = c.sqrt();
  assert(pair_c.first && (pair_c.second == sqrt_c || pair_c.second == -sqrt_c));
  printf("c = %s\n", c.to_string().c_str());
  printf("d = %s\n", d.to_string().c_str());
  printf("c + d = %s\n", (c + d).to_string().c_str());
  printf("c - d = %s\n", (c - d).to_string().c_str());
  printf("c * d = %s\n", (c * d).to_string().c_str());
  printf("c / d = %s\n", (c / d).to_string().c_str());
  printf("sqrt(c) = %s\n", pair_c.second.to_string().c_str());

  printf("--------- Finite Field Test Passed ---------\n");
}

void IntegerTest() {
  printf("--------- Integer Test ---------\n");
  Integer pow_2_3 = pow(2, 3);
  Integer pow_3_4 = pow(3, 4);
  Integer powm_2_3_5 = powm(2, 3, 5);
  Integer int_3 {3};
  Integer int_8 {8};
  Integer int_81 {81};
  assert(pow_2_3 == int_8);
  assert(pow_3_4 == int_81);
  assert(powm_2_3_5 == int_3);

  printf("2 ^ 3 = %s\n", pow_2_3.to_string().c_str());
  printf("3 ^ 4 = %s\n", pow_3_4.to_string().c_str());
  printf("2 ^ 3 mod 5 = %s\n", powm_2_3_5.to_string().c_str());

  assert(SIDH_P % 4 == 3);
  printf("SIDH_P = %s\n", SIDH_P.to_string().c_str());
  printf("--------- Integer Test Passed ---------\n");

//  printf("random number #1: %s\n", PRNG::prng.random(5).to_string().c_str());
//  printf("random number #2: %s\n", PRNG::prng.random(5).to_string().c_str());
//  printf("random number #3: %s\n", PRNG::prng.random(10).to_string().c_str());
//  printf("random number #4: %s\n", PRNG::prng.random(10).to_string().c_str());
//  printf("random number #5: %s\n", PRNG::prng.random(SIDH_P).to_string().c_str());
//  printf("random number #6: %s\n", PRNG::prng.random(SIDH_P).to_string().c_str());
}