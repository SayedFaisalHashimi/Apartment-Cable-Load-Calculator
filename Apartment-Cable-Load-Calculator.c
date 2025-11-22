#include <stdio.h>
#include<cstdlib>

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
    sum_apartment_kw(aptCount, flatCount, kw, cosphi);

    return 0;

}


// -------------------- FUNCTIONS --------------------

// Compute unit power with diversity formula
float compute_unit_power(float kw) {
    return (kw >= 8) ? (0.6 * 8 + 0.4 * (kw - 8)) : (0.6 * kw);
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
        printf("Invalid number of apartments (1-1000).\n");
        exit(1);
    }

     *aptCount = apt;

    
    // Input number of flats per apartment
    for (int i = 0; i < *aptCount; i++) {
        printf("Enter number of units for apartment %d: ", i + 1);
        if(scanf("%d", &flatCount[i])!= 1 || flatCount[i] < 0 || flatCount[i] > 100) {
            printf("Invalid flats number for apartment %d (0-1000).\n", i + 1);
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

    printf("Enter the apartment numbers that have machines: ");
    for(int i = 0; i < *aptWithMachineCount; i++) {
        if(scanf("%d", &aptMachineIndex[i]) != 1 || aptMachineIndex[i] <= 0 || aptMachineIndex[i] > aptCount) {
            printf("Invalid apartment number.\n");
            exit(1);
        }
    }

    /* Input KW and cos phi values for each machine */
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
    printf("\n===== Unit KW Summary =====\n");
    printf("\n");
    float unitPower = 0;
    for(int i = 0; i < *aptCount; i++){
        for(int j = 0; j < flatCount[i]; j++)
        {
            float unitPower = compute_unit_power(kw[i][j]);
            printf("Apartment %d, unit %d → Diversified Power: %.2f kW\n", i+1, j+1, unitPower);
        }
    }

}


// Sum KW per apartment using compute_unit_power
float sum_apartment_kw(int aptCount, int flatCount[], float kw[][MAX_FLAT], float cosphi[]) {
    printf("\n===== Apartment KW Summary =====\n");
    for(int i = 0; i < aptCount; i++) {
        float totalKW = 0.0f;
        for(int j = 0; j < flatCount[i]; j++) {
            totalKW += compute_unit_power(kw[i][j]);
        }
        
        if (flatCount[i]>0 && flatCount[i] < 3)
          {
            totalKW=totalKW/cosphi[i];
              printf("Apartment %d → Total Diversified KW: %.2f kW\n", i+1, totalKW);
          }
          
          else if (flatCount[i]>=3 && flatCount[i] <= 5)
          {
              totalKW*=0.45/cosphi[i];
              printf("Apartment %d → Total Diversified KW: %.2f kW\n", i+1, totalKW);
          }
          
          else if (flatCount[i]>=6 && flatCount[i] <= 10)
          {
              totalKW*=0.43/cosphi[i];
              printf("Apartment %d → Total Diversified KW: %.2f kW\n", i+1, totalKW);
          }
          
          else if (flatCount[i]>=11 && flatCount[i] <= 15)
          {
              totalKW*=0.41/cosphi[i];
              printf("Apartment %d → Total Diversified KW: %.2f kW\n", i+1, totalKW);
          }
          
          else if (flatCount[i]>=16 && flatCount[i] <= 20)
          {
              totalKW*=0.39/cosphi[i];
              printf("Apartment %d → Total Diversified KW: %.2f kW\n", i+1, totalKW);
          }
          
          else if (flatCount[i]>=21 && flatCount[i] <= 25)
          {
              totalKW*=0.36/cosphi[i];
              printf("Apartment %d → Total Diversified KW: %.2f kW\n", i+1, totalKW);
          }
          
          else if (flatCount[i]>=26 && flatCount[i] <= 30)
          {
              totalKW*=0.34/cosphi[i];
              printf("Apartment %d → Total Diversified KW: %.2f kW\n", i+1, totalKW);
          }
          
          else if (flatCount[i]>=31 && flatCount[i] <= 35)
          {
              totalKW*=0.31/cosphi[i];
              printf("Apartment %d → Total Diversified KW: %.2f kW\n", i+1, totalKW);
          }
          
          else if (flatCount[i]>=36 && flatCount[i]<= 40)
          {
              totalKW*=0.29/cosphi[i];
              printf("Apartment %d → Total Diversified KW: %.2f kW\n", i+1, totalKW);
          }
          
          else if (flatCount[i]>=41 && flatCount[i] <= 45)
          {
              totalKW*=0.28/cosphi[i];
              printf("Apartment %d → Total Diversified KW: %.2f kW\n", i+1, totalKW);
          }
          
          else if (flatCount[i]>=46 && flatCount[i] <= 50)
          {
              totalKW*=0.26/cosphi[i];
              printf("Apartment %d → Total Diversified KW: %.2f kW\n", i+1, totalKW);
          }
          
          else if (flatCount[i]>=51 && flatCount[i] <= 55)
          {
              totalKW*=0.25/cosphi[i];
              printf("Apartment %d → Total Diversified KW: %.2f kW\n", i+1, totalKW);
          }
          
          else if (flatCount[i]>=56 && flatCount[i] <= 61)
          {
              totalKW*=0.24/cosphi[i];
              printf("Apartment %d → Total Diversified KW: %.2f kW\n", i+1, totalKW);
          }
          
          else if (flatCount[i]>=62)
          {
              totalKW*=0.23/cosphi[i];
              printf("Apartment %d → Total Diversified KW: %.2f kW\n", i+1, totalKW);
          }
          else 
          {
               printf(" Invalid number of Units. Exited,");
          }
       
    }
    printf("===============================\n");
    return 0.0f; // just to satisfy return type
}
