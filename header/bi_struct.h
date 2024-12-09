#ifndef BI_STRUCT_H
#define BI_STRUCT_H

#include <stdint.h>


#define WORD_BITLEN 32

#if (WORD_BITLEN == 8)
    typedef uint8_t word; // 워드 타입 정의
    typedef uint8_t msg; // 메시지 타입 정의

    // bigint 구조체 정의
    typedef struct {
        int sign;        // 부호: 1이면 양수, -1이면 음수
        int word_len;    // 배열 크기
        word* a;          // 배열 시작 주소
    } bigint;
#endif

#if (WORD_BITLEN == 32)
    typedef uint32_t word; // 워드 타입 정의
    typedef uint32_t msg; // 메시지 타입 정의

    typedef struct {
        int sign;        // 부호: 1이면 양수, -1이면 음수
        int word_len;    // 배열 크기
        word* a;          // 배열 시작 주소
    } bigint;
#endif


#if (WORD_BITLEN == 64)
    typedef uint64_t word; // 워드 타입 정의
    typedef uint64_t msg; // 메시지 타입 정의

    typedef struct {
        int sign;        // 부호: 1이면 양수, -1이면 음수
        int word_len;    // 배열 크기
        word* a;          // 배열 시작 주소
    } bigint;
#endif

typedef bigint* pbigint; // 이중 포인터 사용

#endif // BI_STRUCT_H
