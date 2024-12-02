#ifndef bi_arith_H
#define bi_arith_H

#include "bi_struct.h"
#include "bi_function.h"

int bi_compare_abs(const pbigint A, const pbigint B);
void ADD_ABC(msg A, msg B, msg c, msg* c_out, msg* C);
void ADDC(const pbigint A, const pbigint B, pbigint* C);
void bi_add(const pbigint A, const pbigint B, pbigint* C);

void SUB_ABC(msg A, msg B, msg* b, msg* C);
void SUB_ABB(msg A, msg B, msg b, msg* b_out, msg* C);
void SUBC(const pbigint A, const pbigint B, pbigint* C);
void SUB(const pbigint A, const pbigint B, pbigint* C);

void mul_single_word(word A, word B, pbigint* result);
void MUL_AB(word A, word B, word* C_high, word* C_low);
void MULC(const pbigint A, const pbigint B, pbigint* C);
void MUL(const pbigint A, const pbigint B, pbigint* C);
void MUL_kara(const pbigint x, const pbigint y, pbigint* z);

void div_long_binary(const pbigint A, const pbigint B, pbigint* Q, pbigint* R);
void DIVC(const pbigint A, const pbigint B, pbigint* Q, pbigint* R);
void DIVCC(const pbigint A, const pbigint B, pbigint* Q, pbigint* R);

#endif
