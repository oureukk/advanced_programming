#include <stdio.h>
#include <stdlib.h>
#include "bi_struct.h"
#include "bi_function.h"
#include "bi_arith.h"
#include "bi_add.h"

msg bi_add(pbigint* result, const pbigint a, const pbigint b) {
    // ��ȣ�� ���� ��� ���� ����
    if (a->sign == b->sign) {
        int max_len = (a->word_len > b->word_len) ? a->word_len : b->word_len;
        bi_new(result, max_len + 1); // �ڸ��ø��� ����� �ִ� ���� + 1

        (*result)->sign = a->sign; // ��� ��ȣ ����
        int carry = 0; // �ڸ��ø� �ʱ�ȭ

        // �ڸ��� ���� ���� ����
        for (int i = 0; i < max_len; i++) {
            msg a_i = (i < a->word_len) ? a->a[i] : 0;
            msg b_i = (i < b->word_len) ? b->a[i] : 0;

            msg sum = a_i + b_i + carry;
            if (sum < a_i || sum < b_i) {
                carry = 1; // �ڸ��ø� �߻�
            }
            else {
                carry = 0; // �ڸ��ø� ����
            }
            (*result)->a[i] = sum;
        }

        // ���� �ڸ��ø� ó��
        if (carry) {
            (*result)->a[max_len] = 1;
            (*result)->word_len = max_len + 1;
        }
        else {
            (*result)->word_len = max_len;
        }

    }
    else {
        // ��ȣ�� �ٸ� ��� ���� ����
        if (a->sign < 0) {
            // a�� �����̸� a + b == -(|a| - |b|)
            return bi_subtract(result, b, a);
        }
        else {
            // b�� �����̸� a + b == a - |b|
            return bi_subtract(result, a, b);
        }
    }

    bi_refine(result); // �ʿ� ���� 0 ����

    return 0;
}
