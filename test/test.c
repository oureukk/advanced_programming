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

void test_check_exp(pbigint* pnum1, pbigint* pnum2, pbigint* pnum3, pbigint* result, int oper) {
    const char* fname = "test2.txt";
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

    test_write(pnum3, fp);

    test_write(result, fp);

    fprintf(fp, "%d\n", oper);

    printf("write OK\n");
    fclose(fp);
}

void test_ran()
{
    pbigint a = NULL;
    pbigint b = NULL;
    pbigint c = NULL;
    pbigint result1 = NULL;
    pbigint result2 = NULL;
    pbigint result3 = NULL;
    pbigint result4 = NULL;

    int large_word_len = 256 / sizeof(word);
    //1920/sizeof(word)

    // 큰 정수 초기화
    bi_get_random(&a, large_word_len);
    bi_get_random(&b, large_word_len);
    bi_get_random(&c, large_word_len);
    
    clock_t start,end;
    double cpu_time_used;
    //덧셈과 뺄셈 수행
    bi_add(a, b, &result1);
    test_check(&a, &b, &result1, 1); // 덧셈 결과 기록

    SUB(a, b, &result1);
    test_check(&a, &b, &result1, 2); 

    start=clock();
    MUL(a, b, &result1);
    end=clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("MUL Time: %f seconds\n", cpu_time_used);
    test_check(&a, &b, &result1, 3);

    start=clock();
    MUL_kara(a, b, &result1);
    end=clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("MUL_kara Time: %f seconds\n", cpu_time_used);
    test_check(&a, &b, &result1, 3);
    
    DIVC(a, b, &result1, &result2);
    test_check(&a, &b, &result1, 4);

    start = clock();
    ltr(a, b, c, &result3);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("LTR Time: %f seconds\n", cpu_time_used);
    test_check_exp(&a, &b, &c, &result1, 5);

    start = clock();
    rtl(a, b, c, &result4);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("RTL Time: %f seconds\n", cpu_time_used);
    test_check_exp(&a, &b, &c, &result1, 6);

    // 메모리 해제
    bi_delete(&a);
    bi_delete(&b);
    bi_delete(&c);
    bi_delete(&result1);
    bi_delete(&result2);
    bi_delete(&result3);
    bi_delete(&result4);
}

int main() {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < 10; i++)
    {
        test_ran();

    }
    return 0;
}

// rtl, ltr, bi_mod 테스트 함수
/*int main() {
    pbigint base = NULL;
    pbigint exponent = NULL;
    pbigint modulus = NULL;
    pbigint result = NULL;
    pbigint result2 = NULL;

    // 큰 정수 초기화
    int large_word_len = 1920 / sizeof(word);

    bi_get_random(&base, large_word_len);
    bi_get_random(&exponent, large_word_len);
    bi_get_random(&modulus, large_word_len);

    // ltr 함수 호출 전에 시간 측정
    clock_t start_time = clock();

    // LTR 모듈러 거듭제곱 계산
    ltr(base, exponent, modulus, &result);
    clock_t end_time = clock();
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("ltr function took %.4f seconds to execute\n", time_taken);
    // 결과가 있다면 결과 출력
    if (result != NULL) {
        printf("Result: ");
        bi_print(&result, 16); // 결과 출력 (16진수)
    }
    else printf("no result error\n");

    // RTL 모듈러 거듭제곱 계산
    rtl(base, exponent, modulus, &result2);

    clock_t end_time = clock();
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("rtl function took %.4f seconds to execute\n", time_taken);

    // 결과가 있다면 결과 출력
    if (result2 != NULL) {
        printf("Result: ");
        bi_print(&result2, 16); // 결과 출력 (16진수)
    }
    else printf("no result error\n");

    // 문제 없음
    //bi_mod(base, exponent, &modulus);

    // 메모리 해제
    bi_delete(&base);
    bi_delete(&exponent);
    bi_delete(&modulus);
    bi_delete(&result);
    bi_delete(&result2);

    return 0;
}*/