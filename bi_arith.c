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
    return 0; // 두 절댓값이 같음
}

msg bi_subtract(pbigint* result, const pbigint a, const pbigint b) {
    // 절댓값 비교
    int cmp = bi_compare_abs(a, b);
    if (cmp == 0) {
        // 절댓값이 같으면 결과는 0
        bi_new(result, 1);
        (*result)->sign = 1;
        (*result)->a[0] = 0;
        return 0;
    }

    // 큰 수와 작은 수 결정
    const pbigint larger = (cmp > 0) ? a : b;
    const pbigint smaller = (cmp > 0) ? b : a;
    int sign = (cmp > 0) ? a->sign : -a->sign; // 큰 값의 부호 결정

    // 메모리 초기화 및 부호 설정
    bi_new(result, larger->word_len);
    (*result)->sign = sign;

    // 자리수 단위 뺄셈
    int borrow = 0; // 빌림 초기화
    for (int i = 0; i < larger->word_len; i++) {
        msg a_i = larger->a[i];
        msg b_i = (i < smaller->word_len) ? smaller->a[i] : 0;

        msg C;
        if (a_i < b_i + borrow) {
            C = (a_i + (1ULL << 32)) - b_i - borrow; // 빌림 처리
            borrow = 1; // 빌림 발생
        } else {
            C = a_i - b_i - borrow;
            borrow = 0; // 빌림 없음
        }

        (*result)->a[i] = C; // 결과에 저장
    }

    bi_refine(result);

    return 0;
}
