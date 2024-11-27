#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bi_struct.h"
#include "array.h"
#include "bi_arith.h"

int bi_compare_abs(const pbigint A, const pbigint B) {
    if (A->word_len > B->word_len) {
        return 1;  // A가 크다
    }
    if (A->word_len < B->word_len) {
        return -1; // B가 크다
    }
    for (int i = A->word_len - 1; i >= 0; i--) {
        if (A->a[i] > B->a[i]) {
            return 1;  // A가 크다
        }
        if (A->a[i] < B->a[i]) {
            return -1; // B가 크다
        }
    }
    return 0;  // A와 B가 같다
}

// Algorithm 5: ADD_ABC
void ADD_ABC(msg A, msg B, msg c, msg* c_out, msg* C) {

    msg sum = A + B; // A와 B 더하기
    printf("Initial sum: %x, A: %08x, B: %08x\n", sum, A, B);
    *c_out = 0;
    if (sum < A) { // A + B에서 오버플로우 발생
        *c_out = 1;
        printf("Overflow detected after A+B\n");
    }

    sum += c; // 캐리 더하기
    if (sum < c) { // 캐리 추가로 오버플로우 발생
        *c_out += 1;
        printf("Overflow detected after adding carry\n");
    }

    *C = sum; // 최종 합계 저장오
    printf("Final sum: %x, Carry out: %d\n", *C, *c_out);
}


void ADDC(const pbigint A, const pbigint B, pbigint* C) {
    int n = A->word_len;
    int m = B->word_len;
    int max_len = (n > m) ? n : m;  // 최대 워드 길이 계산
    int min_len = (n < m) ? n : m;  // 최소 워드 길이 계산

    bi_new(C, max_len + 1);  // 최대 워드 길이 + 1 워드를 할당

    msg carry = 0;  // 초기 캐리 값
    for (int j = 0; j < min_len; j++) {
        // 워드 단위로 덧셈 수행

        ADD_ABC(A->a[j], B->a[j], carry, &carry, &(*C)->a[j]);
    }

    for (int j = min_len; j < max_len; j++) {
        // 길이가 긴 쪽의 나머지 워드 처리
        if (n > m) {
            ADD_ABC(A->a[j], 0, carry, &carry, &(*C)->a[j]);
        }
        else {
            ADD_ABC(0, B->a[j], carry, &carry, &(*C)->a[j]);
        }
    }

    // 마지막 캐리 처리
    if (carry != 0) {
        (*C)->a[max_len] = carry;
        (*C)->word_len = max_len + 1;
    }
    else {
        (*C)->word_len = max_len;
    }
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

    if (A->sign > 0 && B->sign < 0) {
        // 절댓값 비교
        if (bi_compare_abs(A, B) >= 0) {
            SUBC(A, B, C);       // A >= B인 경우 A - |B|
            (*C)->sign = 1;
        }
        else {
            SUBC(B, A, C);       // A < B인 경우 |B| - A
            (*C)->sign = -1;
        }
        return;
    }

    if (A->sign < 0 && B->sign > 0) {
        // 절댓값 비교
        if (bi_compare_abs(A, B) > 0) {
            SUBC(A, B, C);       // |A| > B인 경우 |A| - B
            (*C)->sign = -1;
        }
        else {
            SUBC(B, A, C);       // |A| <= B인 경우 B - |A|
            (*C)->sign = 1;
        }
        return;
    }

    // 부호가 같은 경우
    if (A->sign == B->sign) {
        if (A->word_len >= B->word_len) {
            ADDC(A, B, C);
        }
        else {
            ADDC(B, A, C);
        }
        (*C)->sign = A->sign;
    }
}

void SUB_ABC(msg A, msg B, msg* b, msg* C) {
    *b = 0;                  // 초기 빌림은 0
    *C = A - B;              // 기본 뺄셈
    if (A < B) {             // 빌림 발생
        *b = 1;
    }
}

void SUB_ABB(msg A, msg B, msg b, msg* b_out, msg* C) {
    *b_out = 0;              // 초기 빌림은 0
    *C = A - b;              // A에서 빌림 먼저 처리
    if (A < b) {             // 빌림으로 오버플로우 발생
        *b_out = 1;
    }
    *C -= B;                 // 남은 B를 처리
    if (*C > (A - b)) {      // B 처리 후 빌림 발생
        *b_out += 1;
    }
}

void SUBC(const pbigint A, const pbigint B, pbigint* C) {
    int n = A->word_len;     // A의 워드 길이
    int m = B->word_len;     // B의 워드 길이

    bi_new(C, n);            // 결과를 저장할 메모리 생성

    msg b = 0;               // 초기 빌림은 0
    for (int j = 0; j < m; j++) {
        // A[j] - B[j] - b 수행
        SUB_ABB(A->a[j], B->a[j], b, &b, &(*C)->a[j]);
    }

    // A의 나머지 워드 처리 (B의 길이보다 긴 경우)
    for (int j = m; j < n; j++) {
        SUB_ABB(A->a[j], 0, b, &b, &(*C)->a[j]);
    }

    (*C)->word_len = n;      // 결과의 워드 길이 설정
    bi_refine(C);            // 상위 0 제거
}

void SUB(const pbigint A, const pbigint B, pbigint* C) {
    // Case: A == 0
    if (A->word_len == 0 || (A->word_len == 1 && A->a[0] == 0)) {
        bi_assign(C, &B);
        (*C)->sign = -B->sign;  // 결과는 -B
        return;
    }

    // Case: B == 0
    if (B->word_len == 0 || (B->word_len == 1 && B->a[0] == 0)) {
        bi_assign(C, &A);
        return;  // 결과는 A
    }

    // Case: A == B
    if (A->sign == B->sign && A->word_len == B->word_len) {
        int equal = 1;
        for (int i = 0; i < A->word_len; i++) {
            if (A->a[i] != B->a[i]) {
                equal = 0;
                break;
            }
        }
        if (equal) {
            bi_new(C, 1);
            (*C)->a[0] = 0;  // 결과는 0
            (*C)->sign = 1;  // 양수로 설정
            return;
        }
    }

    // Case: 0 < B <= A
    if (A->sign > 0 && B->sign > 0) {
        if (A->word_len > B->word_len || (A->word_len == B->word_len && A->a[A->word_len - 1] >= B->a[B->word_len - 1])) {
            SUBC(A, B, C);
        }
        else {
            SUBC(B, A, C);
            (*C)->sign = -1;  // 결과는 음수
        }
        return;
    }

    // Case: 0 > A >= B
    if (A->sign < 0 && B->sign < 0) {
        if (A->word_len > B->word_len || (A->word_len == B->word_len && A->a[A->word_len - 1] <= B->a[B->word_len - 1])) {
            SUBC(B, A, C);
        }
        else {
            SUBC(A, B, C);
            (*C)->sign = -1;  // 결과는 음수
        }
        return;
    }

    // Case: A > 0 and B < 0 (Addition)
    if (A->sign > 0 && B->sign < 0) {
        B->sign = 1;
        bi_add(A, B, C);
        B->sign = -1;

        return;
    }

    // Case: A < 0 and B > 0 (Addition with sign)
    if (A->sign < 0 && B->sign > 0) {
        A->sign = 1;
        bi_add(A, B, C);
        (*C)->sign = -1;  // 결과는 음수
        A->sign = -1;

        return;
    }
}

void mul_single_word(word A, word B, pbigint* result) {
    const int half_word = 32 / 2;    // 워드를 절반으로 나눔
    const word mask = (1UL << half_word) - 1; // 하위 비트를 위한 마스크

    word A0 = A & mask;                       // A의 하위 절반
    word A1 = A >> half_word;                 // A의 상위 절반
    word B0 = B & mask;                       // B의 하위 절반
    word B1 = B >> half_word;                 // B의 상위 절반

    // 중간 곱셈 결과 계산
    word T0 = A0 * B1;                        // A의 하위 × B의 상위
    word T1 = A1 * B0;                        // A의 상위 × B의 하위

    // 중간 곱셈 결과 합산 및 캐리 처리
    T0 = T0 + T1;
    T1 = (T0 < T1);                           // 캐리 확인

    // C0와 C1 계산
    word C0 = A0 * B0;                        // 하위 워드 곱셈
    word C1 = A1 * B1;                        // 상위 워드 곱셈

    // 결과 병합
    word T = C0;
    C0 += (T0 << half_word);                  // T0를 하위 워드에 병합
    C1 += (T1 << half_word) + (T0 >> half_word) + (C0 < T); // 상위 워드 병합

    // 결과 저장
    (*result)->a[0] = C0;                     // 하위 워드 저장
    (*result)->a[1] = C1;                     // 상위 워드 저장
}


void MUL_AB(word A, word B, word* C_high, word* C_low) {
    bigint* temp_result = NULL;
    bi_new(&temp_result, 2); // 결과 저장 공간 생성

    mul_single_word(A, B, &temp_result);

    *C_low = temp_result->a[0]; // 하위 워드
    *C_high = temp_result->a[1]; // 상위 워드

    bi_delete(&temp_result); // 메모리 해제
}

void MULC(const pbigint A, const pbigint B, pbigint* C) {
    int n = A->word_len;
    int m = B->word_len;

    bi_new(C, n + m);  // 결과 워드 공간 생성
    for (int i = 0; i < n + m; i++) {
        (*C)->a[i] = 0;  // 초기화
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {

            word C_high, C_low;
            // A[i] * B[j]의 곱셈 결과를 C_high, C_low로 분리
            MUL_AB(A->a[i], B->a[j], &C_high, &C_low);

            msg carry = 0;  // 전체 연산에서 사용할 캐리

            // 하위 워드 더하기
            ADD_ABC((*C)->a[i + j], C_low, carry, &carry, &(*C)->a[i + j]);
            // 상위 워드 더하기 (이전 carry 포함)
            ADD_ABC((*C)->a[i + j + 1], C_high, carry, &carry, &(*C)->a[i + j + 1]);

            // 추가 캐리를 상위 워드로 전파
            int k = i + j + 2;  // 상위 워드 시작 인덱스
            while (carry > 0) { // 캐리가 0이 될 때까지 전파
                ADD_ABC((*C)->a[k], 0, carry, &carry, &(*C)->a[k]);
                k++;
            }
        }
    }

    bi_refine(C);  // 상위 0 제거
}

void MUL(const pbigint A, const pbigint B, pbigint* C){
    // A = 0 or B = 0
    if (A->word_len == 0 || B->word_len == 0) {
        bi_new(C, 1);
        (*C)->a[0] = 0;
        (*C)->sign = 1;
        return;
    }

    // A = ±1
    if (A->word_len == 1 && A->a[0] == 1) {
        bi_assign(C, &B);
        (*C)->sign = A->sign * B->sign;
        return;
    }

    // B = ±1
    if (B->word_len == 1 && B->a[0] == 1) {
        bi_assign(C, &A);
        (*C)->sign = A->sign * B->sign;
        return;
    }
    printf("%d\n",A);

    // 일반 곱셈
    pbigint abs_A = NULL , abs_B = NULL;
    bi_new(&abs_A, A->word_len);
    bi_new(&abs_B, B->word_len);
    bi_assign(&abs_A, &A);
    bi_assign(&abs_B, &B);
    abs_A->sign = 1;
    abs_B->sign = 1;

    MULC(abs_A, abs_B, C);
    (*C)->sign = A->sign * B->sign;

    bi_delete(&abs_A);
    bi_delete(&abs_B);
}
// Binary Long Division
void div_long_binary(const pbigint A, const pbigint B, pbigint* Q, pbigint* R) {
    if (bi_compare_abs(A, B) < 0) {
        bi_new(Q, 1);
        bi_new(R, A->word_len);
        bi_assign(R, &A);
        (*Q)->a[0] = 0;
        return;
    }

    int n = A->word_len * WORD_BITLEN;  // 전체 비트 길이 계산
    bi_new(Q, A->word_len);
    bi_new(R, 1);
    (*R)->a[0] = 0;

    for (int i = n - 1; i >= 0; i--) {
        bi_shift_left(R, *R, 1);  // R <<= 1
        if ((A->a[i / WORD_BITLEN] >> (i % WORD_BITLEN)) & 1) {
            (*R)->a[0] |= 1;  // R += (A[i] << 0)
        }

        if (bi_compare_abs(*R, B) >= 0) {
            SUB(*R, B, R);  // R -= B
            (*Q)->a[i / WORD_BITLEN] |= (1 << (i % WORD_BITLEN));  // Q[i] = 1
        }
    }

    bi_refine(Q);
    bi_refine(R);
}

// DIVC implementation
void DIVC(const pbigint A, const pbigint B, pbigint* Q, pbigint* R) {
    if (bi_compare_abs(A, B) < 0) {
        bi_new(Q, 1);
        bi_new(R, A->word_len);
        bi_assign(R, &A);
        (*Q)->a[0] = 0;
        return;
    }

    int k = 0;
    while ((B->a[B->word_len - 1] << k) < (1U << (WORD_BITLEN - 1))) {
        k++;
    }

    pbigint A_prime, B_prime;
    bi_new(&A_prime, A->word_len);
    bi_new(&B_prime, B->word_len);
    bi_shift_left(&A_prime, A, k);  // A' = A * 2^k
    bi_shift_left(&B_prime, B, k);  // B' = B * 2^k

    div_long_binary(A_prime, B_prime, Q, R);  // Use binary division on A' and B'

    bi_shift_right(R, *R, k);  // R = R' / 2^k

    bi_delete(&A_prime);
    bi_delete(&B_prime);
}

// DIVCC: Single-step division assuming A >= B
void DIVCC(const pbigint A, const pbigint B, pbigint* Q, pbigint* R) {
    int n = A->word_len - 1;
    int m = B->word_len - 1;
    unsigned long long high = A->a[n];
    if (n > m) {
        high <<= WORD_BITLEN;
        high |= A->a[n - 1];
    }
    unsigned long long Bm = B->a[m];
    unsigned long long quotient = high / Bm;

    bi_new(Q, 1);
    (*Q)->a[0] = (word)(quotient < (1ULL << WORD_BITLEN) ? quotient : (1ULL << WORD_BITLEN) - 1);

    pbigint product;
    MUL(B, *Q, &product);
    SUB(A, product, R);

    bi_delete(&product);
}
