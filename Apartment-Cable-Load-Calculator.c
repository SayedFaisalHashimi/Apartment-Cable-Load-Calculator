#include <stdio.h>

int main(void) {
    int aptCount = 0;
    int flatCount[20];
    int kw[20][20];
    int i, j;

    // Input number of apartments
    printf("Enter number of apartments: ");
    scanf("%d", &aptCount);

    // Input number of flats per apartment
    for (i = 0; i < aptCount; i++) {
        printf("Enter number of flats for apartment %d: ", i + 1);
        scanf("%d", &flatCount[i]);
    }

    // Input KW usage for each flat
    for (i = 0; i < aptCount; i++) {
        printf("\nApartment %d:\n", i + 1);
        for (j = 0; j < flatCount[i]; j++) {
            printf("Enter KW for apartment %d, flat %d: ", i + 1, j + 1);
            scanf("%d", &kw[i][j]);
        }
    }


    return 0;
}
