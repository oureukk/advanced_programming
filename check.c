#include "bi_struct.h"

void test_write(pbigint* pnum, FILE* fp)
{
    if ((*pnum)->sign == -1) {
        fprintf(fp, "-");
    }
    int first = 1;  // 앞쪽의 0을 생략
    for (int i = (*pnum)->word_len - 1; i >= 0; i--) {
        if (first && (*pnum)->a[i] == 0) {
            continue;
        }
        first = 0;

            fprintf(fp, "%08x", (*pnum)->a[i]);  // 16진법

    }

    if (first) {
        fprintf(fp, "0");
    }

    fprintf(fp, " ");
}

void test_check(pbigint* pnum1, pbigint* pnum2, pbigint* result,int oper)
{
 
    const char* fname = "test.txt"; // or tmpnam(NULL);

    FILE* fp;

    fopen_s(&fp, fname, "a");

    test_write(pnum1, fp);
    test_write(pnum2, fp);
    
    test_write(result, fp);
   

    fprintf(fp,"%d\n", oper);
    rewind(fp);


    printf("wirte OK\n");
    fclose(fp);
}