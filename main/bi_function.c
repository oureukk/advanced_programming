#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bi_function.h"
#include "array.h"
// 메모리 할당 및 초기화 함수 (calloc 사용)
msg bi_new(pbigint* dst, int word_len) {
    if (*dst != NULL) {
        if ((*dst)->a != NULL) {
            free((*dst)->a);  // 기존에 할당된 메모리가 있으면 해제
        }
        free(*dst);  // 기존 구조체 해제
    }

    *dst = (pbigint)calloc(1, sizeof(bigint));
    if (*dst == NULL) {
        fprintf(stderr, "bigint 구조체 메모리 할당 실패\n");
        exit(1); // 프로그램 종료
    }
    (*dst)->word_len = word_len;
    (*dst)->a = (word*)calloc(word_len, sizeof(word));
    if ((*dst)->a == NULL) {
        fprintf(stderr, "bigint 배열 메모리 할당 실패\n");
        free(*dst);
        exit(1); // 프로그램 종료
    }
    (*dst)->sign = 0;
    return 0;
}

// 메모리 해제 함수
msg bi_delete(pbigint* dst) {
    if (*dst != NULL) {
        if ((*dst)->a != NULL) {
            free((*dst)->a);  // 메모리 해제
            (*dst)->a = NULL;  // 포인터를 NULL로 설정
        }
        free(*dst);  // 구조체 자체 해제
        *dst = NULL; // 포인터 NULL 처리
    }
    return 0;
}

// 메모리 재설정 함수
msg bi_refine(pbigint* dst) {
    // 상위 워드가 0인 경우 유효 워드 길이를 계산
    int new_word_len = (*dst)->word_len;
    while (new_word_len > 1 && (*dst)->a[new_word_len - 1] == 0) {
        new_word_len--;
    }

    // 필요한 크기로 다시 할당
    msg* new_a = (msg*)realloc((*dst)->a, new_word_len * sizeof(msg));
    if (new_a == NULL) {
        fprintf(stderr, "Memory reallocation failed\n");
        bi_delete(dst);
        exit(1);  // 프로그램 종료
    }

    (*dst)->a = new_a;  // 포인터 업데이트
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
    array_rand((*dst)->a, word_len);

    bi_refine(dst); // 상위 0 제거
    return 0;
}

// 진법에 맞춰 bigint 출력하는 함수
msg bi_print(pbigint* dst, int base) {
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
            printf("%x", (*dst)->a[i]);  // 16진법
        }  else if (base == 2) {
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
        } else if (base == 16 && ((str[i] >= 'a' && str[i] <= 'f') || (str[i] >= 'A' && str[i] <= 'F'))) {
            digit_value = (str[i] >= 'a') ? 10 + (str[i] - 'a') : 10 + (str[i] - 'A');
        } else {
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
} // bi_function.c 파일
