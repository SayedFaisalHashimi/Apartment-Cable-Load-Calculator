#include <stdio.h>

int main(void) {
    int aptCount = 0;
    int flatCount[1000];
    float kw[1000][100];
    float cosphi[1000];
    int i, j;


    // Input number of apartments
    printf("Enter number of apartments: ");
    if(scanf("%d", &aptCount)!= 1 || aptCount <= 0 || aptCount > 1000) {
        printf("Invalid number of apartments (1-1000).\n");
        return 1;
    };


    /* Machines (optional, building-level extra loads) */
    char isMachine = 'n';
    int machineCount = 0;
    float machineKW[50] = {0.0f};


    // Input number of flats per apartment
    for (i = 0; i < aptCount; i++) {
        printf("Enter number of flats for apartment %d: ", i + 1);
        if(scanf("%d", &flatCount[i])!= 1 || flatCount[i] < 0 || flatCount[i] > 100) {
            printf("Invalid flats number for apartment %d (0-1000).\n", i + 1);
            return 1;
        }
    }

    // Input KW usage for each flat
    for (i = 0; i < aptCount; i++) {
        printf("\nApartment %d:\n", i + 1);
        for (j = 0; j < flatCount[i]; j++) {
            printf("Enter KW for apartment %d, flat %d: ", i + 1, j + 1);
            if(scanf("%f", &kw[i][j]) != 1 || kw[i][j] < 0.0f) {
                printf("Invalid KW value. Must be non-negative.\n");
                return 1;
            }
        }
    }

    // Input: power factor (cos φ) per apartment
    printf("\n--- Power Factor Input ---\n");
    for (i = 0; i < aptCount; i++) {
        printf("Enter cos φ for apartment %d: ", i + 1);
        if(scanf("%f", &cosphi[i])!= 1 || cosphi[i] <= 0.0f || cosphi[i] > 1.0f) {
            printf("Invalid cos φ for apartment %d. Using default 1.0.\n", i + 1);
            cosphi[i] = 1.0f;
        }
    }


    /* --- Optional extra machines (building-level) --- */
    printf("\nIs there any extra machine? (y for yes / n for no): ");
    /* Leading space in format to skip previous newline */
    scanf(" %c", &isMachine);

    if (isMachine == 'y' || isMachine == 'Y') {
        printf("Enter number of extra machines: ");
        if (scanf("%d", &machineCount) != 1 || machineCount < 0 || machineCount > 50) {
            printf("Invalid machine count (0-50).\n");
            return 1;
        }
        for (i = 0; i < machineCount; i++) {
            printf("  Enter load (KW) for machine %d: ", i + 1);
            if (scanf("%f", &machineKW[i]) != 1 || machineKW[i] < 0.0f) {
                printf("Invalid KW value for machine %d.\n", i + 1);
                return 1;
            }
        }
    }


    return 0;
}
