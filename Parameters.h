#pragma once

#include "FiniteField.h"

// p = 2^a * 3^b +/- 1
// p mod 4 = 3

struct Parameters {
    Integer p;
    int a, b;
}
