//
// Created by yww on 2022/5/20.
//

#include "RunAllTests.h"
#include "GmpTest.h"
#include "FiniteFieldTest.h"
#include "CurveTest.h"

void RunAllTests() {
  GmpTest();
  IntegerTest();
  FiniteFieldTest();
  CurveTest();
}