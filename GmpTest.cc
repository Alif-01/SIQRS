//
// Created by yww on 2022/5/20.
//

#include "GmpTest.h"
#include <cstdio>
#include "gmp.h"

void GmpTest() {
  printf("--------- GMP TEST ---------\n");
  mpz_t a, b, sum, mul;
  mpz_init_set_str(a, "12345", 10);
  mpz_init_set_str(b, "54321", 10);
  mpz_init(sum);
  mpz_init(mul);
  mpz_add(sum, a, b);
  mpz_mul(mul, a, b);
  char *str_a = mpz_get_str(NULL, 10, a);
  char *str_b = mpz_get_str(NULL, 10, b);
  char *str_sum = mpz_get_str(NULL, 10, sum);
  char *str_mul = mpz_get_str(NULL, 10, mul);
  printf("a = %s\n", str_a);
  printf("b = %s\n", str_b);
  printf("a + b = %s\n", str_sum);
  printf("a * b = %s\n", str_mul);
  printf("--------- GMP TEST PASSED ---------\n");
}