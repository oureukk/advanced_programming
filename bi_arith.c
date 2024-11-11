#include <stdio.h>
#include <stdlib.h>
#include "bi_struct.h"
#include "bi_function.h"
#include "bi_arith.h"

msg bi_add(pbigint* result, const pbigint a, const pbigint b) {
    // 부호가 같은 경우 덧셈 수행
    if (a->sign == b->sign) {
        int max_len = (a->word_len > b->word_len) ? a->word_len : b->word_len;
        bi_new(result, max_len + 1); // 자리올림을 대비해 최대 길이 + 1

        (*result)->sign = a->sign; // 결과 부호 설정
        int carry = 0; // 자리올림 초기화

        // 자리수 단위 덧셈 수행
        for (int i = 0; i < max_len; i++) {
            msg a_i = (i < a->word_len) ? a->a[i] : 0;
            msg b_i = (i < b->word_len) ? b->a[i] : 0;

            msg sum = a_i + b_i + carry;
            if (sum < a_i || sum < b_i) {
                carry = 1; // 자리올림 발생
            }
            else {
                carry = 0; // 자리올림 없음
            }
            (*result)->a[i] = sum;
        }

        // 최종 자리올림 처리
        if (carry) {
            (*result)->a[max_len] = 1;
            (*result)->word_len = max_len + 1;
        }
        else {
            (*result)->word_len = max_len;
        }

    }
    else {
        // 부호가 다른 경우 뺄셈 수행
        if (a->sign < 0) {
            // a가 음수이면 a + b == -(|a| - |b|)
            return bi_subtract(result, b, a);
        }
        else {
            // b가 음수이면 a + b == a - |b|
            return bi_subtract(result, a, b);
        }
    }

    bi_refine(result); // 필요 없는 0 제거

    return 0;
}
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
