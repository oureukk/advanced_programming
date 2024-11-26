#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bi_function.c"
#include "bi_arith.c"
#include "array.c"

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
        fprintf(fp, "%x", (*pnum)->a[i]);
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

int main() {

    pbigint a = NULL;
    pbigint b = NULL;
    pbigint result1 = NULL;

    int large_word_len = 10;

    // 큰 정수 초기화
    bi_get_random(&a, large_word_len);
    bi_get_random(&b, large_word_len);

    // 덧셈과 뺄셈 수행
    bi_add(a,b,&result1);


    // 결과를 파일에 기록
    test_check(&a, &b, &result1, 1); // 덧셈 결과 기록


    // 메모리 해제
    bi_delete(&a);
    bi_delete(&b);
    bi_delete(&result1);


    return 0;
}
