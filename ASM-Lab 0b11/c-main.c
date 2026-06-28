#include <stdio.h>
#include <stdint.h>

extern void faktorialy(long *pole, int N);
extern int maska(char *str);
extern int spocitani_jednicek(int maska);

int main(void) {
    printf("Faktorialy\n");

    long faktorialy_pole[] = {3, 5, 8, 420};
    int N_faktorial = 4;
    faktorialy(faktorialy_pole , N_faktorial);

    for (int i = 0; i < N_faktorial; i++) {
        printf("%ld ", faktorialy_pole[i]);
    }
    
    printf("\n\n");

    printf("Znaky abecedy\n");

    char *text = "appsy mi opravdu davaji obcas zabrat";
    char *text2 = "kocici zradlo";

    int mask = maska(text);
    int chybejici = spocitani_jednicek(mask);
    printf("Retezec: '%s'\nChybi pismen: %d\n\n", text, chybejici);

}