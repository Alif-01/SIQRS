#pragma once

#include "FiniteField.h"

// p = 2 ^ 372 * 3 ^ 239 - 1
// p mod 4 = 3

const Integer SIDH_P = pow(2, 372) * pow(3, 239) - 1;