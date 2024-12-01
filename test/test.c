#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bi_function.h"
#include "bi_arith.h"
#include "array.h"

void test_write(pbigint* pnum, FILE* fp) {
    if ((*pnum)->sign == -1) {
        fprintf(fp, "-");
    }

    int first = 1; // 유효한 숫자 확인
    for (int i = (*pnum)->word_len - 1; i >= 0; i--) {
        if (first && (*pnum)->a[i] == 0) {
            continue; // 처음 유효한 값 전까지 0 생략
        }
        first = 0;
        fprintf(fp, "%08x", (*pnum)->a[i]);
    }

    if (first) { // 모든 값이 0인 경우
        fprintf(fp, "0");
    }

    fprintf(fp, " ");
}

void test_check(pbigint* pnum1, pbigint* pnum2, pbigint* result, int oper) {
    const char* fname = "test.txt";
    static int first_call = 1;
    FILE* fp;

    if (first_call) {
        fopen_s(&fp, fname, "w");
        first_call = 0;
    }
    else
        fopen_s(&fp, fname, "a");


    if (fp == NULL) {
        perror("파일 열기 실패");
        return;
    }

    test_write(pnum1, fp);

    test_write(pnum2, fp);

    test_write(result, fp);

    fprintf(fp, "%d\n", oper);

    printf("write OK\n");
    fclose(fp);
}



void test_ran()
{
    pbigint a = NULL;
    pbigint b = NULL;
    pbigint result1 = NULL;
    pbigint result2 = NULL;

    int large_word_len = 1920 / sizeof(word);
    //1920/sizeof(word)

    // 큰 정수 초기화
    bi_get_random(&a, large_word_len);
    bi_get_random(&b, large_word_len);

    //덧셈과 뺄셈 수행
    bi_add(a, b, &result1);
    test_check(&a, &b, &result1, 1); // 덧셈 결과 기록

    SUB(a, b, &result1);
    test_check(&a, &b, &result1, 2); 

    MUL(a, b, &result1);
    test_check(&a, &b, &result1, 3);

    DIVC(a, b, &result1, &result2);
    test_check(&a, &b, &result1, 4);

    // 메모리 해제
    bi_delete(&a);
    bi_delete(&b);
    bi_delete(&result1);
    bi_delete(&result2);

}

int main() {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < 10; i++)
    {
        test_ran();

    }
    return 0;
}
