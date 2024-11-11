#ifndef bi_arith_H
#define bi_arith_H

#include "bi_struct.h"
#include "bi_function.h"

void bi_add(pbigint* result, const pbigint a, const pbigint b);
int bi_compare_abs(const pbigint a, const pbigint b);
void bi_subtract(pbigint* result, const pbigint a, const pbigint b);

#endif
