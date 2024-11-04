#include <stdio.h>
#include <stdlib.h>
#include "bi_struct.h"
#include "bi_function.h"

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

msg bi_subtract(pbigint* result, const pbigint a, const pbigint b) {
    // 절댓값이 같은 경우 0을 반환하도록 초기화
    if (bi_compare_abs(a, b) == 0) {
        bi_set_from_array(result, 1, 1, (msg[]){0});
        return 0;
    }

    // 큰 값과 작은 값을 비교하여 결정, 큰 값의 부호를 설정
    const pbigint larger = (bi_compare_abs(a, b) > 0) ? a : b;
    const pbigint smaller = (larger == a) ? b : a;
    int sign = (larger == a) ? a->sign : -a->sign;

    // 결과 할당 및 부호 설정
    bi_new(result, larger->word_len);
    (*result)->sign = sign;

    // 자리수 단위 뺄셈 및 빌림 관리
    int borrow = 0;
    for (int i = 0; i < larger->word_len; i++) {
        msg a_i = larger->a[i];
        msg b_i = (i < smaller->word_len) ? smaller->a[i] : 0;

        msg diff = a_i - b_i - borrow;
        if (a_i < b_i + borrow) {
            diff += (1U << 32);  // 빌림 처리
            borrow = 1;
        } else {
            borrow = 0;
        }

        (*result)->a[i] = diff;
    }

    // 상위 0 제거를 위해 bi_refine 사용
    bi_refine(result);

    return 0;
}
