#include <stdio.h>
#include <stdlib.h>
#include "bi_struct.h"
#include "bi_function.h"
#include "bi_arith.h"
#include "bi_add.h"

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
