#include <stdio.h>

int main(void) {
    int aptCount = 0;
    int flatCount[20];
    float kw[20][20];
    float cosphi[20];
    int i, j;


    // Input number of apartments
    printf("Enter number of apartments: ");
    scanf("%d", &aptCount);


    /* Machines (optional, building-level extra loads) */
    char isMachine = 'n';
    int machineCount = 0;
    float machineKW[50] = {0.0f};


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
            scanf("%f", &kw[i][j]);
        }
    }

    // Input: power factor (cos φ) per apartment
    printf("\n--- Power Factor Input ---\n");
    for (i = 0; i < aptCount; i++) {
        printf("Enter cos φ for apartment %d: ", i + 1);
        scanf("%f", &cosphi[i]);
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
