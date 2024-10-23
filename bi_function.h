#ifndef bi_function_H
#define bi_function_H

#include "bi_struct.h"


msg bi_new(pbigint* dst, int word_len);
msg bi_set_from_array(pbigint* dst, int sign, int word_len, word* a);
msg bi_set_from_string(pbigint* dst, const char* str, int base);
msg bi_get_random(pbigint* dst, int word_len);
msg bi_delete(pbigint* dst);
msg bi_refine(pbigint* dst);
msg bi_assign(pbigint* dst, const pbigint* src);
msg bi_print(pbigint* dst, int base);



#endif