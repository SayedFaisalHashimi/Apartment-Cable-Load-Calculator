#include <stdio.h>
#include<cstdlib>

#define MAX_APT 1000
#define MAX_FLAT 100
#define MAX_MACH 50


/* Function prototypes */
void input_data(int *aptCount, int flatCount[], float kw[][MAX_FLAT], float cosphi[], int *machineCount, float machineKW[]);



int main(void) {
    int   aptCount = 0;
    int   flatCount[MAX_APT] = {0};
    float kw[MAX_APT][MAX_FLAT] = {{0}};
    float cosphi[MAX_APT] = {0};
    int machineCount = 0;
    float machineKW[MAX_MACH] = {0.0f};


     /* --- Input --- */
    input_data(&aptCount, flatCount, kw, cosphi, &machineCount, machineKW);


    return 0;

}





/* --- IMPLEMENTATIONS --- */

void input_data(int *aptCount, int flatCount[], float kw[][MAX_FLAT], float cosphi[],int *machineCount, float machineKW[]){ 
    // Input number of apartments
    int apt;
    printf("Enter number of apartments: ");
    if(scanf("%d", &apt)!= 1 || apt <= 0 || apt > MAX_APT ) {
        printf("Invalid number of apartments (1-1000).\n");
        exit(1);
    }

     *aptCount = apt;

    
    


    // Input number of flats per apartment
    for (int i = 0; i < *aptCount; i++) {
        printf("Enter number of flats for apartment %d: ", i + 1);
        if(scanf("%d", &flatCount[i])!= 1 || flatCount[i] < 0 || flatCount[i] > 100) {
            printf("Invalid flats number for apartment %d (0-1000).\n", i + 1);
            exit(1);
        }
    }

    // Input KW usage for each flat
    for (int i = 0; i < *aptCount; i++) {
        printf("\nApartment %d:\n", i + 1);
        for (int j = 0; j < flatCount[i]; j++) {
            printf("Enter KW for apartment %d, flat %d: ", i + 1, j + 1);
            if(scanf("%f", &kw[i][j]) != 1 || kw[i][j] < 0.0f) {
                printf("Invalid KW value. Must be non-negative.\n");
                exit(1);
            }
        }
    }

    // Input: power factor (cos φ) per apartment
    printf("\n--- Power Factor Input ---\n");
    for (int i = 0; i < *aptCount; i++) {
        printf("Enter cos φ for apartment %d: ", i + 1);
        if(scanf("%f", &cosphi[i])!= 1 || cosphi[i] <= 0.0f || cosphi[i] > 1.0f) {
            printf("Invalid cos φ for apartment %d. Using default 1.0.\n", i + 1);
            cosphi[i] = 1.0f;
        }
    }


    /* --- Optional extra machines (building-level) --- */
    char isMachine = 'n';
    printf("\nIs there any extra machine? (y for yes / n for no): ");
    /* Leading space in format to skip previous newline */
    scanf(" %c", &isMachine);

    if (isMachine == 'y' || isMachine == 'Y') {
        printf("Enter number of extra machines: ");
        if (scanf("%d", machineCount) != 1 || *machineCount < 0 || *machineCount > MAX_MACH) {
            printf("Invalid machine count (0-50).\n");
            return ;
        }
        for (int i = 0; i < *machineCount; i++) {
            printf("  Enter load (KW) for machine %d: ", i + 1);
            if (scanf("%f", &machineKW[i]) != 1 || machineKW[i] < 0.0f) {
                printf("Invalid KW value for machine %d.\n", i + 1);
                exit(1);
            }
        }
    }
    else {
        *machineCount = 0;
    }
}

