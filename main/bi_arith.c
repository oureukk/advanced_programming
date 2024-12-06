#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
    *c_out = 0;
    if (sum < A) { // A + B에서 오버플로우 발생
        *c_out += 1;
    }

    sum += c; // 캐리 더하기
    if (sum < c) { // 캐리 추가로 오버플로우 발생
        *c_out += 1;
    }

    *C = sum; // 최종 합계 저장오
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
        if (bi_compare_abs(A, B) >= 0) {
            SUBC(A, B, C);       // A >= B인 경우 A - |B|
            (*C)->sign = 1;
        } else {
            SUBC(B, A, C);       // A < B인 경우 |B| - A
            (*C)->sign = -1;
        }
        return;
    }

    if (A->sign < 0 && B->sign > 0) {
        if (bi_compare_abs(A, B) > 0) {
            SUBC(A, B, C);       // |A| > B인 경우 |A| - B
            (*C)->sign = -1;
        } else {
            SUBC(B, A, C);       // |A| <= B인 경우 B - |A|
            (*C)->sign = 1;
        }
        return;
    }

    // 부호가 같은 경우
    if (A->sign == B->sign) {
        if (A->word_len >= B->word_len) {
            ADDC(A, B, C);
        } else {
            ADDC(B, A, C);
        }
        (*C)->sign = A->sign;
    }
}

void SUB_ABB(msg A, msg B, msg b, msg* b_out, msg* C) {
    *b_out = 0;             // 초기 빌림은 0

    *C = A - b;              // A에서 빌림 먼저 처리

    if (A < b && b == 0) {             // 빌림으로 오버플로우 발생
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
        if ((bi_compare_abs(A, B) > 0)) {
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
        if ((bi_compare_abs(A, B) < 0)) {
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
    const int half_word = WORD_BITLEN / 2;    // 워드를 절반으로 나눔
    const word mask = (1ULL << half_word) - 1; // 하위 비트를 위한 마스크

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

    // 일반 곱셈
    pbigint abs_A = NULL , abs_B = NULL;
    bi_new(&abs_A, A->word_len);
    bi_new(&abs_B, B->word_len);
    bi_assign(&abs_A, &A);
    bi_assign(&abs_B, &B);


    MULC(abs_A, abs_B, C);
    (*C)->sign = A->sign * B->sign;

    bi_delete(&abs_A);
    bi_delete(&abs_B);
}
void square_single_word(word A, pbigint* result) {
    const int half_word = WORD_BITLEN / 2;    // 워드를 절반으로 나눔
    const word mask = (1ULL << half_word) - 1; // 하위 비트를 위한 마스크

    // 상위와 하위 워드 분리
    word A0 = A & mask;                         // 하위 워드
    word A1 = A >> half_word;                   // 상위 워드

    // (1) 하위 워드 제곱 (C0)
    word C0 = A0 * A0;

    // (2) 상위 워드 제곱 (C1)
    word C1 = A1 * A1;

    // (3) 교차 항 계산
    word T = A0 * A1;                           



    word low = C0 + (T << half_word +1);         
    word carry_low = (low < C0);                


    // 상위 워드에 교차 항 상위 비트와 캐리 추가
    word high = C1 + (T >> half_word-1) + carry_low;

    // (5) 결과 저장
    (*result)->a[0] = low;                         // 하위 워드 저장
    (*result)->a[1] = high;                        // 상위 워드 저장
}
void SQR_AB(word A, word* C_high, word* C_low) {
    bigint* temp_result = NULL;
    bi_new(&temp_result, 2); // 결과 저장 공간 생성

    square_single_word(A, &temp_result);

    *C_low = temp_result->a[0]; // 하위 워드
    *C_high = temp_result->a[1]; // 상위 워드

    bi_delete(&temp_result); // 메모리 해제
}


void SQRC(const pbigint A, pbigint* C) {
    int n = A->word_len;

    bigint *C1 = NULL;
    bigint *C2 = NULL;

    bi_new(&C1, n +n);  // 결과 워드 공간 생성
    bi_new(&C2, n +n);  // 결과 워드 공간 생성
    C1->sign = 1;
    C2->sign = 1;

    for (int i = 0; i < n; i++) {
        word C1_high, C1_low;
        // A[i] * B[j]의 곱셈 결과를 C_high, C_low로 분리
        SQR_AB(A->a[i], &C1_high, &C1_low);

        msg carry = 0; 

        // 하위 워드 더하기
        ADD_ABC((C1)->a[i*2], C1_low, carry, &carry, &(C1)->a[i*2]);
        // 상위 워드 더하기 (이전 carry 포함)
        ADD_ABC((C1)->a[i*2 + 1], C1_high, carry, &carry, &(C1)->a[i*2 + 1]);


        // //추가 캐리를 상위 워드로 전파
        int k = i  + 2;  // 상위 워드 시작 인덱스
        while (carry > 0) { // 캐리가 0이 될 때까지 전파
            ADD_ABC((C1)->a[k], 0, carry, &carry, &(C1)->a[k]);
            k++;
        }

        for (int j = i+1; j < n ; j++) {
            word C2_high, C2_low;
            MUL_AB(A->a[i], A->a[j], &C2_high, &C2_low);
                
            msg carry = 0;  // 전체 연산에서 사용할 캐리

            // 하위 워드 더하기
            ADD_ABC((C2)->a[i + j], C2_low, carry, &carry, &(C2)->a[i + j]);
            // 상위 워드 더하기 (이전 carry 포함)
            ADD_ABC((C2)->a[i + j + 1], C2_high, carry, &carry, &(C2)->a[i + j + 1]);

            // //추가 캐리를 상위 워드로 전파
            int k = i + j + 2;  // 상위 워드 시작 인덱스
            while (carry > 0) { // 캐리가 0이 될 때까지 전파
                ADD_ABC((C2)->a[k], 0, carry, &carry, &(C2)->a[k]);
                k++;
            }

        }
    }


    bi_shift_left(&C2, C2, 1);
    bi_print(&C2, 16);
    bi_new(C, n  +n + 1);

    bi_add(C1, C2, C);

    bi_delete(&C1);
    bi_delete(&C2);
    bi_refine(C);  // 상위 0 제거

}
void MUL_kara(const pbigint x, const pbigint y, pbigint* z) {

    if (x->word_len == 0 || y->word_len == 0) {
    bi_new(z, 1);
    (*z)->a[0] = 0;
    (*z)->sign = 1;
    return;
    }

    int n = x->word_len;
    int m = y->word_len;

    // 기본 곱셈 처리
    if (n <= 1 || m <= 1) {
        MULC(x, y, z);
        return;
    }

    bi_new(z, n + m);
    if (*z == NULL) {
        fprintf(stderr, "Error: Memory allocation for z failed\n");
        return;
    }
    
    int l = ((n >= m ? n : m) + 1) / 2;

    // 중간 변수 초기화
    pbigint A1 = NULL, A0 = NULL, B1 = NULL, B0 = NULL, C = NULL;
    pbigint T1 = NULL, T0 = NULL;
    pbigint S1 = NULL, S0 = NULL, S = NULL;

    bi_new(&A1, x->word_len - l);
    bi_new(&A0, l);
    bi_new(&B1, y->word_len - l);
    bi_new(&B0, l);
    bi_new(&T1, n+m);
    bi_new(&T0, n+m);
    bi_new(&S1, l);
    bi_new(&S0, l);
    bi_new(&S, n+m);
    bi_new(&C, n+m);

    // A1 = x >> 1  
    bi_assign_kara(&A1, x);
    A1->sign = 1;

    bi_shift_right(&A1, l);
    
    // A0 = x % (2^l)
    bi_assign_kara(&A0, x);
    for (int i = l; i < A0->word_len; i++) {
        A0->a[i] = 0;  // 상위 워드를 0으로 초기화
    }
    bi_refine(&A0);
    A0->sign = 1;

    // B1 = y >> l
    bi_assign_kara(&B1, y);
    bi_shift_right(&B1, l);
    B1->sign = 1;

    // B0 = y % (2^l)
    bi_assign_kara(&B0, y);
    for (int i = l; i < B0->word_len; i++) {
        B0->a[i] = 0;  // 상위 워드를 0으로 초기화
    }
    bi_refine(&B0);
    B0->sign = 1;

    MUL(A1, B1, &T1);
    MUL(A0, B0, &T0);

    

    // z = T1 << (2 * l)
    bi_assign_kara(&C, T1);
    bi_shift_left(&C, C, 2 * l * WORD_BITLEN);
    

    pbigint sum1=NULL;
    bi_new(&sum1, n + m);
    ADDC(C, T0, &sum1);
    
    SUB(A0, A1, &S1);
    SUB(B1, B0, &S0);

    bool sign_S = ((S0)->sign) ^ ((S1)->sign);
    S0->sign = 1;
    S1->sign = 1;

    MUL(S1, S0, &S);

    if (sign_S) {
        S->sign = -1;  // 하나만 음수인 경우 음수 설정
    } else {
        S->sign = 1;   // 부호가 같으면 양수 설정
    }


    pbigint sum2=NULL,sum3=NULL;
    bi_new(&sum2, n + m);
    bi_new(&sum3, n + m);
    if (S->sign < 0) {
        S->sign = -1;  // 음수로 설정
    } else {
        S->sign = 1;   // 양수로 설정
    }

    // T1의 부호 유지
    if (T1->sign < 0) {
        T1->sign = -1;  // 음수로 설정
    } else {
        T1->sign = 1;   // 양수로 설정
    }
    bi_add(T1,S,&sum2);

    if (T0->sign < 0) {
        T0->sign = -1;  // 음수로 설정
    } else {
        T0->sign = 1;   // 양수로 설정
    }

    if (sum2->sign < 0) {
        sum2->sign = -1;  // 음수로 설정
    } else {
        sum2->sign = 1;   // 양수로 설정
    }
    bi_add(T0,sum2,&sum3);

    // C += S << l
    
    pbigint ShiftS=NULL;
    bi_new(&ShiftS, n + m );
    bi_shift_left(&ShiftS, sum3, l * WORD_BITLEN);

    ADDC(sum1, ShiftS, z);  // 최종 결과를 z에 저장

    (*z)->sign = x->sign * y->sign;
    
    bi_delete(&A1);
    bi_delete(&A0);
    bi_delete(&B1);
    bi_delete(&B0);
    bi_delete(&T1);
    bi_delete(&T0);
    bi_delete(&S);
    bi_delete(&S1);
    bi_delete(&S0);
    bi_delete(&C);
    bi_delete(&sum1);
    bi_delete(&sum2);
    bi_delete(&sum3);
    bi_delete(&ShiftS);
}

// Binary Long Division
void div_long_binary(const pbigint A, const pbigint B, pbigint* Q, pbigint* R) {    if (bi_compare_abs(A, B) < 0) {
        bi_new(Q, 1);
        bi_new(R, A->word_len);
        bi_assign(R, &A);
        (*Q)->a[0] = 0;
        return;
    }

    pbigint R_tmp = NULL;
    int n = A->word_len * WORD_BITLEN;  // 전체 비트 길이 계산
    bi_new(Q, A->word_len);
    bi_new(R, B->word_len);
    bi_new(&R_tmp, B->word_len);

    (*R)->a[0] = 0;
    (*R)->sign = 1;
    (*Q)->sign = A->sign * B->sign;
    A->sign = 1;
    B->sign = 1;
    R_tmp->sign = 1;
    for (int i = n - 1; i >= 0; i--) {
        bi_shift_left(R, *R, 1);  // R <<= 1
        if ((A->a[i / WORD_BITLEN] >> (i % WORD_BITLEN)) & 1) {
            (*R)->a[0] |= 1;  // R += (A[i] << 0)
        }

        if (bi_compare_abs(*R, B) >= 0) {  

            (*Q)->a[i / WORD_BITLEN] ^= (1 << (i % WORD_BITLEN));  // Q[i] = 1

            SUB(*R, B, &R_tmp);  // R -= B
            bi_assign(R, &R_tmp);
        }
    }

    bi_refine(Q);
    bi_refine(R);
}

// DIVC implementation
void DIVC(const pbigint A, const pbigint B, pbigint* Q, pbigint* R) {
    if (A->word_len == 0 || B->word_len == 0) {
        bi_new(Q, 1);
        (*Q)->a[0] = 0;
        (*Q)->sign = 1;
        return;
    }

    int k = 0;
    while ((B->a[B->word_len - 1] << k) < (1U << (WORD_BITLEN - 1))) {
        k++;
    }



    // A = ±1
    if (A->word_len == 1 && A->a[0] == 1) {
        bi_assign(Q, &B);
        (*Q)->sign = A->sign * B->sign;
        return;
    }

    // B = ±1
    if (B->word_len == 1 && B->a[0] == 1) {
        bi_assign(Q, &A);
        (*Q)->sign = A->sign * B->sign;
        return;
    }

    pbigint A_prime = NULL;
    pbigint B_prime = NULL;
    bi_new(&A_prime, A->word_len);
    bi_new(&B_prime, B->word_len);
    bi_assign(&A_prime, &A);
    bi_assign(&B_prime, &B);

    A_prime->sign = A->sign;
    B_prime->sign = B->sign;

    div_long_binary(A_prime, B_prime, Q, R);  // Use binary division on A' and B'


    bi_delete(&A_prime);
    bi_delete(&B_prime);
}

// 추가
// 모듈러 연산 함수 (나머지 계산)
void bi_mod(const pbigint dividend, const pbigint divisor, pbigint* remainder) {
    if (divisor->word_len == 0 || (divisor->a[0] == 0 && divisor->word_len == 1)) {
        fprintf(stderr, "Error: Division by zero in bi_mod.\n");
        return;
    }

    // 나머지를 저장할 변수
    pbigint Q = NULL, R = NULL;
    bi_new(&R, divisor->word_len);

    // div_long_binary를 사용하여 나머지를 계산
    div_long_binary(dividend, divisor, &Q, &R);  // 몫은 필요 없고 나머지만 사용

    // 나머지 할당
    bi_assign(remainder, &R);

    // 나머지가 0인 경우 확인 (디버깅용)
    bi_print(remainder, 16);  // 16진수로 출력하여 값 확인

    // 메모리 해제
    bi_delete(&Q);
    bi_delete(&R);
}

// Left-to-Right 모듈러 거듭제곱 함수
void ltr(const pbigint base, const pbigint exponent, const pbigint modulus, pbigint* result) {
    if (modulus->word_len == 0 || (modulus->a[0] == 0 && modulus->word_len == 1)) {
        fprintf(stderr, "Error: Modulus cannot be zero.\n");
        return;
    }

    // 결과값 초기화: result = 1
    bi_new(result, 1);
    (*result)->a[0] = 1;
    (*result)->word_len = 1;
    (*result)->sign = 1;

    // 임시 변수 생성
    pbigint base_copy = NULL;
    pbigint temp_result = NULL;
    bi_new(&base_copy, modulus->word_len);
    bi_new(&temp_result, modulus->word_len);

    // base % modulus로 초기화
    bi_assign(&base_copy, base);
    bi_mod(base_copy, modulus, &base_copy);

    // 이진 지수법 (Binary Exponentiation)
    for (int i = 0; i < exponent->word_len * WORD_BITLEN; i++) {
        // result = (result * result) % modulus
        MUL(*result, *result, &temp_result);
        bi_mod(&temp_result, modulus, result);

        // 현재 비트가 1이면 result = (result * base_copy) % modulus
        if (bi_test_bit(exponent, i)) {
            MUL(*result, base_copy, &temp_result);
            bi_mod(&temp_result, modulus, result);
        }

        // Base를 제곱 (base = base^2)
        MUL(base_copy, base_copy, &temp_result);
        bi_mod(&temp_result, modulus, &base_copy);
    }

    // 임시 변수 정리
    bi_delete(&base_copy);
    bi_delete(&temp_result);
}

// 지수에서 i번째 비트가 1인지 확인하는 함수
// bi_test_bit 예시 구현
int bi_test_bit(const pbigint n, int bit_index) {
    if (bit_index < 0 || bit_index >= n->word_len * WORD_BITLEN) {
        return 0;  // 범위 밖의 비트는 0
    }

    int word_index = bit_index / WORD_BITLEN;
    int bit_position = bit_index % WORD_BITLEN;
    return (n->a[word_index] >> (WORD_BITLEN - bit_position - 1)) & 1;
}
