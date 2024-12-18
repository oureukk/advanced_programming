#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bi_function.h"
#include "array.h"

// 메모리 할당 및 초기화 함수 (calloc 사용)
msg bi_new(pbigint* dst, int word_len) {
    if (*dst != NULL) {
        bi_delete(dst);
    }

    *dst = (pbigint)calloc(1, sizeof(bigint));
    if (*dst == NULL) {
        fprintf(stderr, "bigint 구조체 메모리 할당 실패\n");
        return 1; // 프로그램 종료
    }
    (*dst)->word_len = word_len;
    (*dst)->a = (word*)calloc(word_len, sizeof(word));
    if ((*dst)->a == NULL) {
        fprintf(stderr, "bigint 배열 메모리 할당 실패\n");
        free(*dst);
        return 1; // 프로그램 종료
    }
    (*dst)->sign = 0;
    return 0;
}

// 메모리 해제 함수
void bi_delete(pbigint* dst) {
    if (*dst == NULL)
        return;
#ifdef ZERORIZE
    array_init((*x)->a, (*x)->wordlen);
#endif
    free((*dst)->a);  // 메모리 해제        
    free(*dst);  // 구조체 자체 해제
    *dst = NULL; // 포인터 NULL 처리

}

// 메모리 재설정 함수
msg bi_refine(pbigint* dst) {
    // 상위 워드가 0인 경우 유효 워드 길이를 계산
    int new_word_len = (*dst)->word_len;

    while (new_word_len > 1 && (*dst)->a[new_word_len - 1] == 0) {
        new_word_len--;
    }

    // 배열의 길이가 변경되었다면 메모리 재할당
    if (new_word_len != (*dst)->word_len) {
        msg* new_a = (msg*)calloc(new_word_len, sizeof(msg));
        if (new_a == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
            return 1;
        }
        memcpy(new_a, (*dst)->a, new_word_len * sizeof(msg));
        free((*dst)->a);
        (*dst)->a = new_a;
    }

    (*dst)->word_len = new_word_len;  // 워드 길이 업데이트
    return 0;
}

msg bi_refine_lower(pbigint* dst) {
    // 하위 워드가 0인 경우 유효 워드 길이를 계산
    int new_word_len = (*dst)->word_len;

    // 하위 0비트를 제거
    while (new_word_len > 1 && (*dst)->a[0] == 0) {
        new_word_len--;
    }

    // 배열의 길이가 변경되었다면 메모리 재할당
    if (new_word_len != (*dst)->word_len) {
        msg* new_a = (msg*)calloc(new_word_len, sizeof(msg));
        if (new_a == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
            return 1;
        }
        memcpy(new_a, (*dst)->a, new_word_len * sizeof(msg));
        free((*dst)->a);
        (*dst)->a = new_a;
    }

    (*dst)->word_len = new_word_len;  // 워드 길이 업데이트
    return 0;
}

// 무작위 난수로 bigint 초기화 함수
msg bi_get_random(pbigint* dst, int word_len) {
    bi_new(dst, word_len);

    if (*dst == NULL || (*dst)->a == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    (*dst)->sign = (rand() % 2 == 0) ? 1 : -1; // 양수와 음수를 50% 확률로 설정
    //(*dst)->sign = 1;

    array_rand((*dst)->a, word_len);

    return 0;
}

// 진법에 맞춰 bigint 출력하는 함수
msg bi_print(pbigint* dst, int base) {
    if (dst == NULL || *dst == NULL) {
        printf("Error: NULL pointer passed to bi_print\n");
        return 0;
    }

    if (base < 2 || base > 16) {
        printf("Unsupported base: %d\n", base);
        return 0;
    }

    if ((*dst)->sign == -1) {
        printf("-");
    }

    int first = 1;  // 앞쪽의 0을 생략
    for (int i = (*dst)->word_len - 1; i >= 0; i--) {
        if (first && (*dst)->a[i] == 0) {
            continue;
        }
        first = 0;

        if (base == 16) {
            printf("%08x", (*dst)->a[i]);  // 16진법
        }
        else if (base == 2) {
            for (int j = 31; j >= 0; j--) {
                printf("%d", ((*dst)->a[i] >> j) & 1);
            }
        }
        printf(" ");
    }

    if (first) {
        printf("0");
    }

    printf("\n");
    return 0;
}

// 배열을 받아 bigint 구조체에 설정하는 함수
msg bi_set_from_array(pbigint* dst, int sign, int word_len, word* a) {
    bi_delete(dst);
    bi_new(dst, word_len);

    (*dst)->sign = sign;
    memcpy((*dst)->a, a, word_len * sizeof(word));

    return 0;
}

msg bi_set_from_string(pbigint* dst, const char* str, int base) { //const str을 읽기전용으로
    size_t str_len = strlen(str);

    if (str_len == 0) {
        printf("Empty string\n");
        return 1;
    }
    // 워드 개수 계산 (32비트로 처리)
    size_t word_len = (str_len * 4 + 31) / 32;
    bi_new(dst, word_len);  // 새로운 메모리 할당
    // 부호 처리
    (*dst)->sign = 1;
    if (str[0] == '-') {
        (*dst)->sign = -1;
        str++;  // 부호를 제외하고 숫자 처리
        str_len--;
    }
    // 각 워드에 값을 채움
    msg current_word = 0;
    int bit_position = 0;
    for (int i = str_len - 1; i >= 0; i--) {
        int digit_value;

        if (str[i] >= '0' && str[i] <= '9') {
            digit_value = str[i] - '0';
        }
        else if (base == 16 && ((str[i] >= 'a' && str[i] <= 'f') || (str[i] >= 'A' && str[i] <= 'F'))) {
            digit_value = (str[i] >= 'a') ? 10 + (str[i] - 'a') : 10 + (str[i] - 'A');
        }
        else {
            printf("Invalid character in input string: %c\n", str[i]);
            return 1;  // 오류 반환
        }
        // base에 맞춰 숫자를 현재 워드에 저장
        current_word |= (digit_value << bit_position);
        bit_position += (base == 16 ? 4 : (base == 2 ? 1 : (base == 10 ? 4 : 3)));  // 진법에 따른 비트 이동

        // 워드가 꽉 찼을 경우 배열에 추가
        if (bit_position >= 32) {
            (*dst)->a[--word_len] = current_word;
            current_word = 0;
            bit_position = 0;
        }
    }
    // 남은 값이 있으면 마지막 워드에 추가
    if (bit_position > 0) {
        (*dst)->a[--word_len] = current_word;
    }

    return 0;  // 성공 시 0 반환
}

msg bi_assign(pbigint* dst, const pbigint* src) { //const를 읽기전용으로

    bi_delete(dst);
    bi_new(dst, (*src)->word_len);

    (*dst)->sign = (*src)->sign;
    memcpy((*dst)->a, (*src)->a, (*src)->word_len * sizeof(msg));

    return 0;
}

void bi_assign_kara(pbigint* dst, const pbigint src) {
    if (src == NULL || src->word_len == 0) {
        fprintf(stderr, "Error: Source bigint is NULL or invalid\n");
        return;
    }

    if (*dst != NULL) {
        bi_delete(dst);
    }

    bi_new(dst, src->word_len); // 메모리 재할당
    if (*dst == NULL) {
        fprintf(stderr, "Error: Memory allocation for dst failed\n");
        return;
    }

    (*dst)->sign = src->sign;

    for (int i = 0; i < src->word_len; i++) {
        (*dst)->a[i] = src->a[i];
    }
}

void bi_shift_left(pbigint* result, const pbigint A, int shift) {
    if (shift == 0) {
        bi_assign(result, &A);
        return;
    }

    int word_shift = shift / WORD_BITLEN;   // 워드 단위 시프트
    int bit_shift = shift % WORD_BITLEN;   // 워드 내 비트 시프트
    pbigint tmp = NULL;
    bi_new(&tmp, A->word_len + word_shift + 1); // 새로운 공간 할당
    tmp->sign = 1;
    for (int i = 0; i < word_shift; i++) {
        (tmp)->a[i] = 0; // 워드 단위로 0 초기화
    }

    for (int i = 0; i < A->word_len; i++) {
        (tmp)->a[i + word_shift] = A->a[i];
    }

    if (bit_shift > 0) {
        msg carry = 0;
        for (int i = 0; i < (tmp)->word_len; i++) {
            msg current = (tmp)->a[i];
            (tmp)->a[i] = (current << bit_shift) | carry; // 비트 이동
            carry = current >> (WORD_BITLEN - bit_shift);    // 캐리 값 저장
        }
    }
    bi_refine(&tmp); // 상위 0 제거
    bi_assign(result, &tmp);
    bi_delete(&tmp);
}

/*void bi_shift_right(pbigint* x, int shift)
{
    if (shift < 0) {
        fprintf(stderr, "Error: shift is negative'\n");
        return;
    }
    if (shift >= (*x)->word_len)
        return;

    int new_word_len = (*x)->word_len - shift;

    for (int i = 0; i < new_word_len; i++)
    {
        (*x)->a[i] = (*x)->a[i + shift];
    }
    for (int i = new_word_len; i < (*x)->word_len; i++)
    {
        (*x)->a[i] = 0;
    }

    // Reallocate memory for the new word length
    word* copy = (*x)->a;
    copy = (word*)realloc((*x)->a, new_word_len * sizeof(word));
    if (!copy) {
        fprintf(stderr, "Error: Memory reallocation failed in 'right_shift_word'\n");
        exit(1);
    }
    (*x)->a = copy;
    // Update the word length
    (*x)->word_len = new_word_len;
}*/

void bi_shift_right(pbigint* x, int shift)
{
    if (shift < 0) {
        fprintf(stderr, "Error: shift cannot be negative.\n");
        return;
    }

    if (shift == 0 || (*x)->word_len == 0) {
        // Shift가 0이면 아무것도 하지 않음
        return;
    }

    // Shift가 word_len보다 크면 워드 길이를 0으로 설정
    if (shift >= (*x)->word_len) {
        // 모든 값을 0으로 설정하고 word_len을 0으로 설정
        memset((*x)->a, 0, (*x)->word_len * sizeof(word));
        (*x)->word_len = 0;
        return;
    }

    // 새로운 word_len 계산
    int new_word_len = (*x)->word_len - shift;

    // 배열을 오른쪽으로 shift만큼 이동
    for (int i = 0; i < new_word_len; i++) {
        (*x)->a[i] = (*x)->a[i + shift];
    }

    // 나머지 부분을 0으로 채움
    for (int i = new_word_len; i < (*x)->word_len; i++) {
        (*x)->a[i] = 0;
    }

    // 메모리 재할당: 새로운 word_len에 맞게 메모리 크기 조정
    word* new_a = (word*)realloc((*x)->a, new_word_len * sizeof(word));
    if (!new_a) {
        fprintf(stderr, "Error: Memory reallocation failed in 'bi_shift_right'.\n");
        exit(1);
    }

    (*x)->a = new_a;
    (*x)->word_len = new_word_len;  // 새로운 word_len으로 갱신
}