#ifndef bi_function_H
#define bi_function_H

#include "bi_struct.h"
#include "bi_function.h"

int bi_compare_abs(const pbigint a, const pbigint b);
msg bi_subtract(pbigint* result, const pbigint a, const pbigint b);

#endif