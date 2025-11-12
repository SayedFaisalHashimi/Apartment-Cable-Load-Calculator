#include <stdio.h>
#include<cstdlib>

#define MAX_APT 1000
#define MAX_FLAT 100
#define MAX_MACH 50


/* Function prototypes */
void input_data(int *aptCount, int flatCount[], float kw[][MAX_FLAT], float cosphi[]);
void input_extra_machines(int aptCount, int *machineCount, float machineKW[], int *aptWithMachineCount, int aptMachineIndex[]);
void calculate_unit_power(int *aptCount, int flatCount[], float kw[][MAX_FLAT]);


int main(void) {
    int   aptCount = 0;
    int   flatCount[MAX_APT] = {0};
    float kw[MAX_APT][MAX_FLAT] = {{0}};
    float cosphi[MAX_APT] = {0};
    int machineCount = 0;
    float machineKW[MAX_MACH] = {0.0f};
    int aptWithMachineCount = 0;
    int aptMachineIndex[MAX_MACH] = {0};


     /* --- Input --- */
    input_data(&aptCount, flatCount, kw, cosphi);

     /* --- Extra machines input --- */
    input_extra_machines(aptCount, &machineCount, machineKW, &aptWithMachineCount, aptMachineIndex);

    /* --- Calculate unit power --- */
    calculate_unit_power(&aptCount, flatCount, kw);

    return 0;

}





/* --- IMPLEMENTATIONS --- */

void input_data(int *aptCount, int flatCount[], float kw[][MAX_FLAT], float cosphi[]){ 
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
}


void input_extra_machines(int aptCount, int *machineCount, float machineKW[], int *aptWithMachineCount, int aptMachineIndex[]) {
    printf("\nEnter number of apartments that have extra machines (like Elevator): ");
    if(scanf("%d", aptWithMachineCount) != 1 || *aptWithMachineCount < 0 || *aptWithMachineCount > aptCount) {
        printf("Invalid number of apartments with machines.\n");
        exit(1);
    }

    if(*aptWithMachineCount == 0) {
        *machineCount = 0;
        return;
    }

    printf("Enter the apartment numbers that have machines: ");
    for(int i = 0; i < *aptWithMachineCount; i++) {
        if(scanf("%d", &aptMachineIndex[i]) != 1 || aptMachineIndex[i] <= 0 || aptMachineIndex[i] > aptCount) {
            printf("Invalid apartment number.\n");
            exit(1);
        }
    }

    for(int i = 0; i < *aptWithMachineCount; i++) {
        int aptNum = aptMachineIndex[i];
        printf("Enter Kw for machine in apartment %d: ", aptNum);
        if(scanf("%f", &machineKW[i]) != 1 || machineKW[i] < 0.0f) {
            printf("Invalid KW for machine in apartment %d.\n", aptNum);
            exit(1);
        }

        float cosphiMachine;
        printf("Enter cos phi for machine inside ap %d: ", aptNum);
        if(scanf("%f", &cosphiMachine) != 1 || cosphiMachine <= 0.0f || cosphiMachine > 1.0f) {
            printf("Invalid cos phi for machine in apartment %d. Using default 1.0.\n", aptNum);
            cosphiMachine = 1.0f;
        }
        // Optional: store cosphiMachine if needed later
    }

    *machineCount = *aptWithMachineCount;
}




void calculate_unit_power(int *aptCount, int flatCount[], float kw[][MAX_FLAT])
{
    float unitPower = 0;
    for(int i = 0; i < *aptCount; i++){
        for(int j = 0; j < flatCount[i]; j++)
        {
            if(kw[i][j] >= 8)
            {
                unitPower = 0.6 * 8 + (kw[i][j] - 8) * 0.4;
            }
            else
            {
                unitPower = kw[i][j] * 0.6;
            }

            printf("Calculated power for apartment %d, unit %d = %.2f kW\n", i + 1, j + 1, unitPower);
        }
    }

    exit(1);
}
