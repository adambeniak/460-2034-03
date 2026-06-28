#include <stdio.h>
#include <stdint.h>

int data_x = 0b10101010;
int count = 0;
extern void count_ones(void);

int result_num = 0;
int iterations = 6;
extern void fibb(void);

char text[] = "rXtXzXc";
char from = 'X';
char replace_to = 'e';
extern void replace_char(void);

int main(void) {

    count_ones();
    printf("Ones %d", count);
    printf("\n");

    data_x = 0b111110101010;
    count_ones();
    printf("Ones %d", count);
    printf("\n");

    printf("\n");

    fibb();
    printf("%dth fibbonaci: %d", iterations + 1, result_num);
    printf("\n");
    printf("\n");

    replace_char();
    printf("string: %s", text);
    printf("\n");

    return 0;
}