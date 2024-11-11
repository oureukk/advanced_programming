#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bi_struct.h"
#include "bi_function.h"
#include "bi_arith.h"

// 덧셈 함수
void bi_add(pbigint* result, const pbigint a, const pbigint b) {
    // 두 수의 부호가 같은 경우, 덧셈을 수행
    if (a->sign == b->sign) {
        int max_len = (a->word_len > b->word_len) ? a->word_len : b->word_len;
        bi_new(result, max_len + 1);
        (*result)->sign = a->sign;

        uint64_t carry = 0;
        for (int i = 0; i < max_len; i++) {
            uint64_t a_i = i < a->word_len ? a->a[i] : 0;
            uint64_t b_i = i < b->word_len ? b->a[i] : 0;
            uint64_t sum = a_i + b_i + carry;
            (*result)->a[i] = sum & 0xFFFFFFFF;
            carry = sum >> 32;
        }

        if (carry) {
        (*result)->a[max_len] = carry; }
        bi_refine(result);  // 불필요한 상위 0 제거
    } else {
        // 부호가 다른 경우, 뺄셈 함수를 호출
        bi_subtract(result, a, b);
    }
}

// 절댓값 비교 함수
int bi_compare_abs(const pbigint a, const pbigint b) {
    if (a->word_len != b->word_len) {
        return (a->word_len > b->word_len) ? 1 : -1;
    }
    for (int i = a->word_len - 1; i >= 0; i--) {
        if (a->a[i] != b->a[i]) {
            return (a->a[i] > b->a[i]) ? 1 : -1;
        }
    }
    return 0;
}

// 뺄셈 함수
void bi_subtract(pbigint* result, const pbigint a, const pbigint b) {
    int cmp = bi_compare_abs(a, b);
    const pbigint larger = (cmp >= 0) ? a : b;
    const pbigint smaller = (cmp >= 0) ? b : a;
    int sign = (cmp >= 0) ? a->sign : -a->sign;

    bi_new(result, larger->word_len);
    (*result)->sign = sign;

    int borrow = 0;
    for (int i = 0; i < larger->word_len; i++) {
        uint64_t a_i = larger->a[i];
        uint64_t b_i = (i < smaller->word_len) ? smaller->a[i] : 0;
        uint64_t sub = a_i - b_i - borrow;
        (*result)->a[i] = sub & 0xFFFFFFFF;
        borrow = (a_i < b_i + borrow) ? 1 : 0;
    }

    bi_refine(result);
}

