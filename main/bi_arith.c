#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bi_struct.h"
#include "array.h"

// Algorithm 5: ADD_ABC
void ADD_ABC(msg A, msg B, msg c, msg* c_out, msg* C) {
    msg sum = A + B; // A와 B 더하기
    *c_out = 0;

    if (sum < A) { // A + B에서 오버플로우 발생
        *c_out = 1;
    }

    sum += c; // 캐리 더하기
    if (sum < c) { // 캐리 추가로 오버플로우 발생
        (*c_out)++;
    }

    *C = sum; // 최종 합계 저장
}


// Algorithm 6: ADDC
void ADDC(const pbigint A, const pbigint B, pbigint* C) {
    int n = A->word_len;
    int m = B->word_len;
    int max_len = (n > m) ? n : m;

    // C 초기화 (캐리 고려해서 +1)
    bi_new(C, max_len + 1);

    msg carry = 0;
    for (int j = 0; j < max_len; j++) {
        msg a_val = (j < n) ? A->a[j] : 0; // A의 현재 워드
        msg b_val = (j < m) ? B->a[j] : 0; // B의 현재 워드
        ADD_ABC(a_val, b_val, carry, &carry, &((*C)->a[j]));
    }

    // 최상위 캐리를 저장
    (*C)->a[max_len] = carry;

    // 결과를 정리 (불필요한 상위 0 제거)
    bi_refine(C);
}



// Algorithm 7: ADD
void bi_add(const pbigint A, const pbigint B, pbigint* C) {
    // A와 B 중 하나가 0일 경우
    if (A->word_len == 0) {
        bi_assign(C, &B);
        return;
    }
    if (B->word_len == 0) {
        bi_assign(C, &A);
        return;
    }

    // 부호가 다를 경우 (양수 + 음수 또는 음수 + 양수)
    if (A->sign > 0 && B->sign < 0) {
        pbigint abs_B;
        bi_new(&abs_B, B->word_len);
        bi_assign(&abs_B, &B);
        abs_B->sign = 1; // B를 절댓값으로 바꿈
        // A - |B| 수행
        // SUB(A, abs_B, C); // SUB 함수 호출 필요
        bi_delete(&abs_B);
        return;
    }

    if (A->sign < 0 && B->sign > 0) {
        pbigint abs_A;
        bi_new(&abs_A, A->word_len);
        bi_assign(&abs_A, &A);
        abs_A->sign = 1; // A를 절댓값으로 바꿈
        // B - |A| 수행
        // SUB(B, abs_A, C); // SUB 함수 호출 필요
        bi_delete(&abs_A);
        return;
    }

    // 부호가 같은 경우
    if (A->sign < 0 && B->sign < 0) {
        ADDC(A, B, C);
        (*C)->sign = -1; // 결과 부호는 음수
    } else {
        ADDC(A, B, C);
        (*C)->sign = 1;  // 결과 부호는 양수
    }
}

// 곱셈 연산
void bi_mul(pbigint* result, const pbigint a, const pbigint b)
{
    bi_new(result, a-> word_len + b-> word_len);

    (*result)->sign = (a->sign == b->sign) ? 1 : -1;

    for (int i = 0; i < a->word_len; i++) {
        uint64_t carry = 0;
        for (int j = 0; j < b->word_len; j++) {
            uint64_t a_i = i < a->word_len ? a->a[i] : 0;
            uint64_t a_j = j < b->word_len ? b->a[j] : 0;
            uint64_t tmp = a_i * a_j;
            uint64_t sum = (*result)->a[i + j] + tmp + carry;
            (*result)->a[i + j] = sum & 0xFFFFFFFF;
            carry = sum >> 32;
        }
        if (carry) {
            (*result)->a[i + b->word_len] += carry;
        }
    }
    bi_refine(result);  
}
