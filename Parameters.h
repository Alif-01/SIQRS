#pragma once

#include "FiniteField.h"

// p = 2 ^ 372 * 3 ^ 239 - 1
// p mod 4 = 3

const int K2 = 372, K3 = 239;
const Integer P2 = pow(2, K2), P3 = pow(3, K3);
const Integer SIDH_P = P2 * P3 - 1;