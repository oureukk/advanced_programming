#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> 
#include "bi_struct.h" 

// 배열을 초기화하는 함수
msg array_init(word* a, int word_len) {
    memset(a, 0, word_len * sizeof(word));  // 배열을 0으로 초기화
    word result = 0;

    // 초기화 확인을 위한 검증
    for (int i = 0; i < word_len; i++) {
        result |= a[i];
    }

    // 초기화가 완료되었는지 확인
    if (result == 0) {
        return 0;
    } else {
        fprintf(stderr, "Array initialization failed.\n");
        return 1;
    }
}

// 배열을 복사하는 함수
msg array_copy(word** dst, const word* src, int word_len) {
    if (*dst != NULL) {
        free(*dst);  // 기존 메모리가 있으면 해제
    }

    // 메모리 할당
    *dst = (word*)calloc(word_len, sizeof(word));
    if (*dst == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    // src에서 dst로 데이터 복사
    memcpy(*dst, src, word_len * sizeof(word));
    return 0;
}

// 배열을 무작위 값으로 채우는 함수
msg array_rand(word* dst, int word_len) {
    static int is_seeded = 0; // 시드가 설정되었는지 확인
    if (!is_seeded) {
        srand((unsigned int)time(NULL));
        is_seeded = 1;
    }

    uint8_t* p = (uint8_t*)dst;
    int byte_len = word_len * sizeof(word);

    for (int i = 0; i < byte_len; i++) {
        p[i] = rand() % 0xFF;
    }
    return 0;
}
