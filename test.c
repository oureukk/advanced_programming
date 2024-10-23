#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bi_function.c"

int main() {
    srand((unsigned int)time(NULL));

    pbigint my_bigint = NULL;
    pbigint temp_bigint= NULL;
    pbigint temp2_bigint= NULL;
    pbigint copy_bigint= NULL;

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
    bi_print(&copy_bigint, 16);

    // 동적으로 할당한 메모리 해제
    bi_delete(&my_bigint);
    bi_delete(&temp_bigint);
    bi_delete(&temp2_bigint);
    bi_delete(&copy_bigint);

    return 0;
}
