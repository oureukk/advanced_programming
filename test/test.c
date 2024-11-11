#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bi_function.h"
#include "bi_arith.h"
#include "bi_add.h"

// bi_subtract 함수 테스트 함수
void test_bi_subtract() {
    pbigint a = NULL;
    pbigint b = NULL;
    pbigint result = NULL;

    // 첫 번째 테스트 케이스: 큰 수 - 작은 수 (양수 결과)
    word a_arr[] = {0x12345678, 0x9abcdef0, 0x0fedcba9, 0x87654321, 0x0};
    word b_arr[] = {0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x0};
    bi_set_from_array(&a, 1, 5, a_arr);
    bi_set_from_array(&b, 1, 5, b_arr);

    printf("Test Case 1: 큰 수 - 작은 수 (양수 결과)\n");
    printf("A: ");
    bi_print(&a, 16);
    printf("B: ");
    bi_print(&b, 16);
    bi_subtract(&result, a, b);
    printf("A - B = ");
    bi_print(&result, 16);

    // 메모리 해제
    bi_delete(&a);
    bi_delete(&b);
    bi_delete(&result);

    // 두 번째 테스트 케이스: 작은 수 - 큰 수 (음수 결과)
    word a_arr2[] = {0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x0};
    word b_arr2[] = {0x12345678, 0x9abcdef0, 0x0fedcba9, 0x87654321, 0x0};
    bi_set_from_array(&a, 1, 5, a_arr2);
    bi_set_from_array(&b, 1, 5, b_arr2);

    printf("Test Case 2: 작은 수 - 큰 수 (음수 결과)\n");
    printf("A: ");
    bi_print(&a, 16);
    printf("B: ");
    bi_print(&b, 16);
    bi_subtract(&result, a, b);
    printf("A - B = ");
    bi_print(&result, 16);

    // 메모리 해제
    bi_delete(&a);
    bi_delete(&b);
    bi_delete(&result);

    // 세 번째 테스트 케이스: 같은 값 (결과가 0)
    word a_arr3[] = {0x12345678, 0x9abcdef0, 0x0fedcba9, 0x87654321, 0x0};
    word b_arr3[] = {0x12345678, 0x9abcdef0, 0x0fedcba9, 0x87654321, 0x0};
    bi_set_from_array(&a, 1, 5, a_arr3);
    bi_set_from_array(&b, 1, 5, b_arr3);

    printf("Test Case 3: 같은 값 (결과가 0)\n");
    printf("A: ");
    bi_print(&a, 16);
    printf("B: ");
    bi_print(&b, 16);
    bi_subtract(&result, a, b);
    printf("A - B = ");
    bi_print(&result, 16);

    // 메모리 해제
    bi_delete(&a);
    bi_delete(&b);
    bi_delete(&result);
}

void test_bi_add()
{
    pbigint a = NULL;
    pbigint b = NULL;
    pbigint result = NULL;

    
    word a_arr[] = { 0x12345678, 0x9abcdef0, 0x0fedcba9, 0x87654321, 0x0 };
    word b_arr[] = { 0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x0 };
    bi_set_from_array(&a, 1, 5, a_arr);
    bi_set_from_array(&b, 1, 5, b_arr);

    printf("Add Test 1\n");
    printf("A: ");
    bi_print(&a, 16);
    printf("B: ");
    bi_print(&b, 16);
    bi_add(&result, a, b);
    printf("A + B = ");
    bi_print(&result, 16);

    // 메모리 해제
    bi_delete(&a);
    bi_delete(&b);
    bi_delete(&result);

    // X2
    word a_arr3[] = { 0x12345678, 0x9abcdef0, 0x0fedcba9, 0x87654321, 0x0 };
    word b_arr3[] = { 0x12345678, 0x9abcdef0, 0x0fedcba9, 0x87654321, 0x0 };
    bi_set_from_array(&a, 1, 5, a_arr3);
    bi_set_from_array(&b, 1, 5, b_arr3);

    printf("Add Test 2\n");
    printf("A: ");
    bi_print(&a, 16);
    printf("B: ");
    bi_print(&b, 16);
    bi_add(&result, a, b);
    printf("A + B = ");
    bi_print(&result, 16);

    // 메모리 해제
    bi_delete(&a);
    bi_delete(&b);
    bi_delete(&result);
}

int main() {
    /*srand((unsigned int)time(NULL));

    pbigint my_bigint = NULL;
    pbigint temp_bigint = NULL;
    pbigint temp2_bigint = NULL;
    pbigint copy_bigint = NULL;

    // 무작위 난수로 bigint 생성
    bi_get_random(&my_bigint, 5);

    // 생성된 난수 출력
    printf("16진수: ");
    bi_print(&my_bigint, 16);

    printf("2진수: ");
    bi_print(&my_bigint, 2);

    // 배열로부터 설정
    word array[5] = {0x12345678, 0x9abcdef0, 0x0fedcba9, 0x87654321, 0x0};
    bi_set_from_array(&temp_bigint, 1, 5, array);
    bi_set_from_string(&temp2_bigint, "1A3F", 16);

    printf("배열에서 설정된 bigint:\n");
    bi_print(&temp_bigint, 16);
    bi_print(&temp2_bigint, 16);

    // bigint 복사
    bi_assign(&copy_bigint, &temp_bigint);
    printf("복사된 bigint:\n");
    bi_print(&copy_bigint, 16);

    // 동적으로 할당한 메모리 해제
    bi_delete(&my_bigint);
    bi_delete(&temp_bigint);
    bi_delete(&temp2_bigint);
    bi_delete(&copy_bigint);
    */
   
    // bi_subtract 테스트 실행
    test_bi_subtract();
    test_bi_add();
    

    return 0;
}
