//
// Created by yww on 2022/5/20.
//

#include "RunAllTests.h"
#include "GmpTest.h"
#include "FiniteFieldTest.h"

void RunAllTests() {
  GmpTest();
  FiniteFieldTest();
}