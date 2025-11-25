#include <stdio.h>
#include <stdlib.h>

#define MAX_APT 1000
#define MAX_FLAT 100
#define MAX_MACH 50

/*
* ------------------------------------------------------------
* Function Prototypes
* ------------------------------------------------------------
* These tell the compiler what functions exist below so that
* main() can call them even though the full definitions
* appear later in the code.
*/
float compute_unit_power(float kw);  // calculates diversified unit power
void input_data(int *aptCount, int flatCount[], float kw[][MAX_FLAT], float cosphi[]);
void input_extra_machines(int aptCount, int *machineCount, float machineKW[], int *aptWithMachineCount, int aptMachineIndex[]);
void calculate_unit_power(int *aptCount, int flatCount[], float kw[][MAX_FLAT]);
float sum_apartment_kw(int aptCount, int flatCount[], float kw[][MAX_FLAT], float cosphi[]);
float get_diversity_factor(int units);
float total_field_KW(float totalKW);
float calculate_machines_power(float machineKW, float SimultaneityFactor, float cosphiMachine, float effiecencyFactor);


int main(void) {
    /*
* ------------------------------------------------------------
* MAIN VARIABLES
* ------------------------------------------------------------
* These variables store all data entered by the user, such as
* apartment count, number of flats, KW values, and machine data.
*/
    int   aptCount = 0;                         // Total number of apartments
    int   flatCount[MAX_APT] = {0};             // Number of flats inside each apartment
    float kw[MAX_APT][MAX_FLAT] = {{0}};        // KW usage for each flat  
    float cosphi[MAX_APT] = {0};                // Power factor per apartment
    
    int machineCount = 0;                       // Number of machines in total
    float machineKW[MAX_MACH] = {0.0f};         // KW values for machines
    int aptWithMachineCount = 0;                // How many apartments have at least one machine
    int aptMachineIndex[MAX_MACH] = {0};        // The apartment indexes that contain machines

     /* --- Step 1: General user input for apartments and flats --- */
    input_data(&aptCount, flatCount, kw, cosphi);

     /* --- Step 2: Input additional machines (e.g., elevators) --- */
    input_extra_machines(aptCount, &machineCount, machineKW, &aptWithMachineCount, aptMachineIndex);

    /* --- Step 3: Compute the unit power for each flat based on KW usage --- */
    calculate_unit_power(&aptCount, flatCount, kw);

    // Step 4: Sum KW per apartment
    float buildingTotal =sum_apartment_kw(aptCount, flatCount, kw, cosphi);

    printf("\n=== Field Total Diversified KW (including accumulator) : %.2f kW ===\n", buildingTotal);

    return 0;

}


// -------------------- FUNCTIONS --------------------

// Compute unit power with diversity formula
float compute_unit_power(float kw) {
    return (kw >= 8) ? (0.6f * 8.0f + 0.4f * (kw - 8.0f)) : (0.6f * kw);
}


/*
* ------------------------------------------------------------
* Function: input_data
* ------------------------------------------------------------
* Responsible for collecting:
* - Number of apartments
* - Number of flats in each apartment
* - KW consumption of each flat
* - cos phi (power factor) per apartment
* The function also includes error-checking to prevent invalid input.
*/
void input_data(int *aptCount, int flatCount[], float kw[][MAX_FLAT], float cosphi[]){ 

    int apt;
    printf("Enter number of apartments: ");
    if(scanf("%d", &apt)!= 1 || apt <= 0 || apt > MAX_APT ) {
        printf("Invalid number of apartments (1-%d).\n", MAX_APT);
        exit(1);
    }

     *aptCount = apt;

    
    // Input number of flats per apartment
    for (int i = 0; i < *aptCount; i++) {
        printf("Enter number of units for apartment %d: ", i + 1);
        if(scanf("%d", &flatCount[i])!= 1 || flatCount[i] <= 0 || flatCount[i] >  MAX_FLAT) {
            printf("Invalid flats number for apartment %d (1-%d).\n", i + 1, MAX_FLAT);
            exit(1);
        }
    }

    // Input KW usage for each flat
    for (int i = 0; i < *aptCount; i++) {
        printf("\nApartment %d:\n", i + 1);
        for (int j = 0; j < flatCount[i]; j++) {
            printf("Enter KW for apartment %d, unit %d: ", i + 1, j + 1);
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


/*
* ------------------------------------------------------------
* Function: input_extra_machines
* ------------------------------------------------------------
* Handles additional loads such as elevators or water pumps.
* This function collects:
* - How many apartments contain machines
* - Which apartments they are
* - KW and cos phi values of each machine
*/
void input_extra_machines(int aptCount, int *machineCount, float machineKW[], int *aptWithMachineCount, int aptMachineIndex[]) {

    printf("\nEnter number of apartments that have extra machines (like Elevator): ");
    if(scanf("%d", aptWithMachineCount) != 1 || *aptWithMachineCount < 0 || *aptWithMachineCount > aptCount) {
        printf("Invalid number of apartments with machines.\n");
        exit(1);
    }

    /* If there are no machine-equipped apartments, nothing more to do */
    if(*aptWithMachineCount == 0) {
        *machineCount = 0;
        return;
    }

    printf("Enter the apartment numbers that have machines (space separated): ");
    for(int i = 0; i < *aptWithMachineCount; i++) {
        if(scanf("%d", &aptMachineIndex[i]) != 1 || aptMachineIndex[i] <= 0 || aptMachineIndex[i] > aptCount) {
            printf("Invalid apartment number.\n");
            exit(1);
        }
    }

    /* Input KW and parameters values for each machine */
    for(int i = 0; i < *aptWithMachineCount; i++) {
        int aptNum = aptMachineIndex[i];         /* aptNum is 1-based apartment number */

        printf("Enter Kw for machine in apartment %d: ", aptNum);
        if(scanf("%f", &machineKW[i]) != 1 || machineKW[i] < 0.0f) {
            printf("Invalid KW for machine in apartment %d.\n", aptNum);    
            exit(1);
        }


        float cosphiMachine = 1.0f;
        printf("Enter cos phi for machine inside ap %d: ", aptNum);
        if(scanf("%f", &cosphiMachine) != 1 || cosphiMachine <= 0.0f || cosphiMachine > 1.0f) {
            printf("Invalid cos phi for machine in apartment %d. Using default 1.0.\n", aptNum);
            cosphiMachine = 1.0f;
        }
        // Optional: store cosphiMachine if needed later


        float SimultaneityFactor = 1.0f;
        printf("Enter Simultaneity Factor for machine inside ap %d: ", aptNum);
        if(scanf("%f", &SimultaneityFactor) != 1 || SimultaneityFactor <= 0.0f || SimultaneityFactor > 1.0f) {
            printf("Invalid Simultaneity Factor for machine in apartment %d. Using default 1.0.\n", aptNum);
            SimultaneityFactor = 1.0f;
        }

        float effiecencyFactor = 1.0f;
        printf("Enter effiecency Factor for machine inside ap %d: ", aptNum);
        if(scanf("%f", &effiecencyFactor) != 1 || effiecencyFactor <= 0.0f || effiecencyFactor > 1.0f) {
            printf("Invalid effiecency Factor for machine in apartment %d. Using default 1.0.\n", aptNum);
            effiecencyFactor = 1.0f;
        }

        float machinespower = calculate_machines_power(machineKW[i], SimultaneityFactor, cosphiMachine, effiecencyFactor);

        printf("Apartment %d machine → Diversified Power: %.2f kW\n", aptNum, machinespower);
    }

    *machineCount = *aptWithMachineCount;
}


/*
* ------------------------------------------------------------
* Function: calculate_unit_power
* ------------------------------------------------------------
* This function calculates the "unit power" of each flat based
* on its KW usage using a simple diversity rule:
* - For the first 8 kW → multiply by 0.6
* - Anything above 8 kW → multiply by 0.4
* Then the function prints all calculated values.
*/
void calculate_unit_power(int *aptCount, int flatCount[], float kw[][MAX_FLAT])
{
    printf("\n===== Unit KW Summary =====\n\n");

    for(int i = 0; i < *aptCount; i++){
        for(int j = 0; j < flatCount[i]; j++)
        {
            float unitPower = compute_unit_power(kw[i][j]);
            printf("Apartment %d, unit %d → Diversified Power: %.2f kW\n", i+1, j+1, unitPower);
        }
    }

}


/* Return factor based on number of units in the apartment */
float get_diversity_factor(int units) 
{
    if (units <= 0) return 0.0f;
    if (units < 3) return 1.0f;
    if (units <= 5) return 0.45f;
    if (units <= 10) return 0.43f;
    if (units <= 15) return 0.41f;
    if (units <= 20) return 0.39f;
    if (units <= 25) return 0.36f;
    if (units <= 30) return 0.34f;
    if (units <= 35) return 0.31f;
    if (units <= 40) return 0.29f;
    if (units <= 45) return 0.28f;
    if (units <= 50) return 0.26f;
    if (units <= 55) return 0.25f;
    if (units <= 61) return 0.24f;
    return 0.23f; /* 62+ */
}

/* Accumulator for total field KW */
float total_field_KW(float totalKW)
{
    static float sumfieldKW = 0.0f;
    sumfieldKW += totalKW;
    return sumfieldKW;
}


// Sum KW per apartment using compute_unit_power
float sum_apartment_kw(int aptCount, int flatCount[], float kw[][MAX_FLAT], float cosphi[]) {
    printf("\n===== Apartment KW Summary =====\n");
    float buildingTotal = 0.0f;

    for(int i = 0; i < aptCount; i++) {

    float factor = get_diversity_factor(flatCount[i]);

        if (factor == 0.0f) {
            printf("Apartment %d → Invalid number of units.\n", i + 1);
        continue;
        }


        float totalKW = 0.0f;
        for(int j = 0; j < flatCount[i]; j++) {
            totalKW += compute_unit_power(kw[i][j]);
        }
        
       if (cosphi[i] <= 0.0f || cosphi[i] > 1.0f) {
            printf("Warning: cosφ for apartment %d is zero or invalid. Using 1.0 instead to avoid division by zero.\n", i + 1);
            cosphi[i] = 1.0f;
        }


        totalKW = (totalKW / cosphi[i]) * factor;
        printf("Apartment %d → Diversified Power: %.2f kW\n", i+1, totalKW);

        total_field_KW(totalKW);    /* accumulate (stored inside static) */
        buildingTotal += totalKW;
    }

    printf("===============================\n");
    return buildingTotal;
}

float calculate_machines_power(float machineKW, float SimultaneityFactor, float cosphiMachine, float effiecencyFactor) {
    /* Guard against zero or invalid values */
    if(cosphiMachine <= 0.0f) cosphiMachine = 1.0f;
    if(SimultaneityFactor <= 0.0f) SimultaneityFactor = 1.0f;
    if(effiecencyFactor <= 0.0f) effiecencyFactor = 1.0f;

    return (machineKW / cosphiMachine) * SimultaneityFactor / effiecencyFactor;
}
