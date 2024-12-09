#ifndef ARRAY_H
#define ARRAY_H

#include <stdint.h>  
#include "bi_struct.h"

//typedef uint32_t word;

msg array_init(word* a, int word_len);
msg array_copy(word** dst, const word* src, int word_len);
msg array_rand(word* dst, int word_len);

#endif 
