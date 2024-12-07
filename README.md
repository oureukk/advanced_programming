# Algebraic Operations Library

This C library is designed to perform algebraic operations on large integers (bigint) with variable bit lengths. It supports basic arithmetic and additional operations for large numbers.

## Structure Definitions

### `bigint`
- **Fields**:
  - `int sign`: Indicates the sign of the number (1 for positive, -1 for negative).
  - `int word_len`: Indicates the length of the array.
  - `word* a`: Pointer to the start of the data array.

### Data Types
- **`word`**: Can be 8, 32, or 64 bits, corresponding to `uint8_t`, `uint32_t`, and `uint64_t`, respectively.
- **`msg`**: Represents the status of operations, using the same bit length as `word`.

---

## Key Functions

### Initialization and Memory Management
- **`bi_new`**: Initializes a bigint structure.
- **`bi_delete`**: Frees memory allocated for a bigint structure.

### Setup and Conversion
- **`bi_set_from_array`**: Sets a bigint from an array.
- **`bi_set_from_string`**: Sets a bigint from a string.
- **`bi_print`**: Prints a bigint in a specified base.

### Arithmetic Operations
- **`ADD`**: Adds two bigints.
- **`SUB`**: Subtracts one bigint from another.
- **`MUL`**: Multiplies two bigints.
- **`DIVC`**: Divides one bigint by another, returning both quotient and remainder.

### Utilities
- **`bi_get_random`**: Generates a random bigint of specified length.
- **`bi_shift_left`**: Performs a bitwise left shift on a bigint.
- **`bi_shift_right`**: Performs a bitwise right shift on a bigint.
- **`bi_refine`**: Optimizes the bigint structure by trimming unnecessary leading zeros.
- **`bi_assign`**: Copies the content from one bigint to another, ensuring deep copy.

### Advanced Operations
- **`bi_mod`**: Performs the modulus operation between two bigints.
- **`ltr`**: Computes the modular exponentiation of large numbers.
- **`bi_test_bit`**: Tests the value at a specific bit position.

---

## Array Management for using rand();
- **`array_init`**: Initializes an array.
- **`array_copy`**: Copies one array to another.
- **`array_rand`**: Fills an array with random values.

---

## Example Usage: `test.c`

This example demonstrates the use of the library to perform arithmetic operations and log the results in `test.txt`, which can be verified using a Python script `test.py`.

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bi_function.h"
#include "bi_arith.h"
#include "array.h"

void test_write(pbigint* pnum, FILE* fp) {
    // Implementation...
}

void test_check(pbigint* pnum1, pbigint* pnum2, pbigint* result, int oper) {
    // Implementation...
}

void test_ran() {
    // Implementation...
}

int main() {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < 1000; i++) {
        test_ran();
    }
    return 0;
}
