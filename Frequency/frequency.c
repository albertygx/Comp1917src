#include <stdio.h>
#include <stdlib.h>

#define FILE_PATH "hamlet-lf.txt"
#define CODE_LF 10
#define CODE_SPACE 32
#define CODE_CAP_A 65
#define CODE_CAP_E 69
#define CODE_CAP_I 73
#define CODE_CAP_K 75
#define CODE_CAP_O 79
#define CODE_CAP_U 85
#define CODE_CAP_Z 90
#define CODE_LOW_A 97
#define CODE_LOW_E 101
#define CODE_LOW_I 105
#define CODE_LOW_K 107
#define CODE_LOW_O 111
#define CODE_LOW_U 117
#define CODE_LOW_Z 122

int main(int argc, char *argv[]) {
    FILE *txtFile = fopen(FILE_PATH, "r");
    if (txtFile == NULL)
        return -1;
    char c = 0;
    int charCountL = 0;
    int charCountC = 0;
    int charCountLF = 0;
    int charCountV = 0;
    int charCountK = 0;
    int charCountSpace = 0;
    int byteCount = 0;
    while ((c = getc(txtFile)) != EOF) {
        if (CODE_CAP_A <= c && c <= CODE_CAP_Z) {
            charCountC++;
            if (c == CODE_CAP_A || c == CODE_CAP_E || c == CODE_CAP_I
                    || c == CODE_CAP_O || c == CODE_CAP_U) {
                charCountV++;
            }
            if (c == CODE_CAP_K){
                charCountK++;
            }
        } else if (CODE_LOW_A <= c && c <= CODE_LOW_Z) {
            charCountL++;
            if (c == CODE_LOW_A || c == CODE_LOW_E || c == CODE_LOW_I
                    || c == CODE_LOW_O || c == CODE_LOW_U) {
                charCountV++;
            }
            if (c == CODE_LOW_K){
                charCountK++;
            }
        } else if (c == CODE_LF) {
            charCountLF++;
        } else if (c == CODE_SPACE) {
            charCountSpace++;
        }
        byteCount++;
    }
    double per = 0;
    double perK = 0;
    per = (double)charCountV / (charCountL + charCountC);
    perK = (double)charCountK / (charCountL + charCountC);
    printf("byte: %d\n", byteCount);
    printf("upper: %d\n", charCountC);
    printf("lower: %d\n", charCountL);
    printf("LF: %d\n", charCountLF);
    printf("vowel: %lf\n", per);
    printf("K: %.8lf\n", perK);
    printf("Space: %d\n", charCountSpace);
return 0;
}
