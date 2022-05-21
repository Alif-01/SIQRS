//
// Created by yww on 2022/5/21.
//

#include "FiniteFieldTest.h"
#include "FiniteField.h"

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
  assert(a + b == sum_a_b);
  assert(a - b == diff_a_b);
  assert(a * b == prod_a_b);
  assert(a / b == quot_a_b);
  printf("a = %s\n", a.to_string().c_str());
  printf("b = %s\n", b.to_string().c_str());
  printf("a + b = %s\n", (a + b).to_string().c_str());
  printf("a - b = %s\n", (a - b).to_string().c_str());
  printf("a * b = %s\n", (a * b).to_string().c_str());
  printf("a / b = %s\n", (a / b).to_string().c_str());

  // Fp2
  Fp2 c(1, 2, p);
  Fp2 d(3, 4, p);
  Fp2 sum_c_d(4, 6, p);
  Fp2 diff_c_d(9, 9, p);
  Fp2 prod_c_d(6, 10, p);
  Fp2 quot_c_d(0, 8, p);
  assert(c + d == sum_c_d);
  assert(c - d == diff_c_d);
  assert(c * d == prod_c_d);
  assert(c / d == quot_c_d);
  printf("c = %s\n", c.to_string().c_str());
  printf("d = %s\n", d.to_string().c_str());
  printf("c + d = %s\n", (c + d).to_string().c_str());
  printf("c - d = %s\n", (c - d).to_string().c_str());
  printf("c * d = %s\n", (c * d).to_string().c_str());
  printf("c / d = %s\n", (c / d).to_string().c_str());

  printf("--------- Finite Field Test Passed ---------\n");
}