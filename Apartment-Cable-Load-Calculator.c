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
void input_data(struct Building *b);
void input_extra_machines(struct Building *b);
void calculate_unit_power(struct Building *b);
float sum_apartment_kw(struct Building *b);
float get_diversity_factor(int units);
float total_field_KW(float totalKW);
float calculate_machines_power(float machineKW, float SimultaneityFactor, float cosphiMachine, float effiecencyFactor);



/* Data structures */
struct Machine {
    float simultaneity;
    float efficiency;
    float cosphi;
    float kw;
};

struct Unit {
    float kw;
};

struct Apartment {
    int flatCount;
    float cosphi;
    int machineIndex;           /* optional index if using building-level pool (unused here) */
    struct Unit units[MAX_FLAT];
    struct Machine machine;     /* single machine per apartment in this design */
    int hasMachine;             /* 0 or 1 */
};

struct Building {
    int aptCount;
    struct Apartment apts[MAX_APT];
    int machineCount;                 /* number of apartments that have machines */
    int aptMachineIndex[MAX_MACH];    /* 1-based apartment indices that contain machines */
};



int main(void) {
   
    struct Building building = {0};

    
     /* --- Step 1: General user input for apartments and flats --- */
    input_data(&building);

     /* --- Step 2: Input additional machines (e.g., elevators) --- */
    input_extra_machines(&building);

    /* --- Step 3: Compute the unit power for each flat based on KW usage --- */
    calculate_unit_power(&building);

    // Step 4: Sum KW per apartment
    float buildingTotal =sum_apartment_kw(&building);

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
void input_data(struct Building *b){ 

    int apt;
    printf("Enter number of apartments: ");
    if(scanf("%d", &apt)!= 1 || apt <= 0 || apt > MAX_APT ) {
        printf("Invalid number of apartments (1-%d).\n", MAX_APT);
        exit(1);
    }

     b->aptCount = apt;

    
    // Input number of flats per apartment
    for (int i = 0; i < b->aptCount; i++) {
        printf("Enter number of units for apartment %d: ", i + 1);
        if(scanf("%d", &b->apts[i].flatCount)!= 1 || b->apts[i].flatCount <= 0 || b->apts[i].flatCount >  MAX_FLAT) {
            printf("Invalid flats number for apartment %d (1-%d).\n", i + 1, MAX_FLAT);
            exit(1);
        }
        /* initialize hasMachine flag */
        b->apts[i].hasMachine = 0;
    }

    // Input KW usage for each flat
    for (int i = 0; i < b->aptCount; i++) {
        printf("\nApartment %d:\n", i + 1);
        for (int j = 0; j < b->apts[i].flatCount; j++) {
            printf("Enter KW for apartment %d, unit %d: ", i + 1, j + 1);
            if(scanf("%f", &b->apts[i].units[j].kw) != 1 || b->apts[i].units[j].kw < 0.0f) {
                printf("Invalid KW value. Must be non-negative.\n");
                exit(1);
            }
        }
    }

    // Input: power factor (cos φ) per apartment
    printf("\n--- Power Factor Input ---\n");
    for (int i = 0; i < b->aptCount; i++) {
        printf("Enter cos φ for apartment %d: ", i + 1);
        if(scanf("%f", &b->apts[i].cosphi)!= 1 || b->apts[i].cosphi <= 0.0f || b->apts[i].cosphi > 1.0f) {
            printf("Invalid cos φ for apartment %d. Using default 1.0.\n", i + 1);
            b->apts[i].cosphi = 1.0f;
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
void input_extra_machines(struct Building *b) {

    printf("\nEnter number of apartments that have extra machines (like Elevator): ");
    int count;
    if(scanf("%d", &count) != 1 || count < 0 ||  count > b->aptCount) {
        printf("Invalid number of apartments with machines.\n");
        exit(1);
    }

    /* If there are no machine-equipped apartments, nothing more to do */
    if( count == 0) {
        b->machineCount = 0;
        return;
    }

    b->machineCount = count;

    printf("Enter the apartment numbers that have machines (space separated): ");
    for(int i = 0; i < count; i++) {
        if(scanf("%d", &b->aptMachineIndex[i]) != 1 || b->aptMachineIndex[i] <= 0 || b->aptMachineIndex[i] > b->aptCount) {
            printf("Invalid apartment number.\n");
            exit(1);
        }
    }

    /* Input KW and parameters values for each machine */
    for(int i = 0; i < count; i++) {
        int aptNum = b->aptMachineIndex[i];         /* aptNum is 1-based apartment number */
        int idx = aptNum - 1;
        printf("Enter Kw for machine in apartment %d: ", aptNum);
        if(scanf("%f", &b->apts[idx].machine.kw) != 1 || b->apts[idx].machine.kw < 0.0f) {
            printf("Invalid KW for machine in apartment %d.\n", aptNum);    
            exit(1);
        }


        b->apts[idx].machine.cosphi = 1.0f;
        printf("Enter cos phi for machine inside ap %d: ", aptNum);
        if(scanf("%f", &b->apts[idx].machine.cosphi) != 1 || b->apts[idx].machine.cosphi <= 0.0f || b->apts[idx].machine.cosphi > 1.0f) {
            printf("Invalid cos phi for machine in apartment %d. Using default 1.0.\n", aptNum);
            b->apts[idx].machine.cosphi = 1.0f;
        }
        // Optional: store cosphiMachine if needed later


        b->apts[idx].machine.simultaneity = 1.0f;
        printf("Enter Simultaneity Factor for machine inside ap %d: ", aptNum);
        if(scanf("%f", &b->apts[idx].machine.simultaneity) != 1 || b->apts[idx].machine.simultaneity <= 0.0f || b->apts[idx].machine.simultaneity > 1.0f) {
            printf("Invalid Simultaneity Factor for machine in apartment %d. Using default 1.0.\n", aptNum);
            b->apts[idx].machine.simultaneity = 1.0f;
        }

        b->apts[idx].machine.efficiency = 1.0f;
        printf("Enter effiecency Factor for machine inside ap %d: ", aptNum);
        if(scanf("%f", &b->apts[idx].machine.efficiency) != 1 || b->apts[idx].machine.efficiency <= 0.0f || b->apts[idx].machine.efficiency > 1.0f) {
            printf("Invalid effiecency Factor for machine in apartment %d. Using default 1.0.\n", aptNum);
            b->apts[idx].machine.efficiency = 1.0f;
        }

        float machinespower = calculate_machines_power(b->apts[idx].machine.kw,
            b->apts[idx].machine.simultaneity,
            b->apts[idx].machine.cosphi,
            b->apts[idx].machine.efficiency);

        b->apts[idx].hasMachine = 1;
        printf("Apartment %d machine → Diversified Power: %.2f kW\n", aptNum, machinespower);
    }

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
void calculate_unit_power(struct Building *b)
{
    printf("\n===== Unit KW Summary =====\n\n");

    for(int i = 0; i < b->aptCount; i++){
        for(int j = 0; j < b->apts[i].flatCount; j++)
        {
            float unitPower = compute_unit_power(b->apts[i].units[j].kw);
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
float sum_apartment_kw(struct Building *b) {
    printf("\n===== Apartment KW Summary =====\n");
    float buildingTotal = 0.0f;

    for(int i = 0; i < b->aptCount; i++) {

    float factor = get_diversity_factor(b->apts[i].flatCount);

        if (factor == 0.0f) {
            printf("Apartment %d → Invalid number of units.\n", i + 1);
        continue;
        }


        float totalKW = 0.0f;
        for(int j = 0; j < b->apts[i].flatCount; j++) {
            totalKW += compute_unit_power(b->apts[i].units[j].kw);
        }
        
       if (b->apts[i].cosphi <= 0.0f || (b->apts[i].cosphi > 1.0f)){
            printf("Warning: cosφ for apartment %d is zero or invalid. Using 1.0 instead to avoid division by zero.\n", i + 1);
            b->apts[i].cosphi = 1.0f;
        }


        totalKW = totalKW / (b->apts[i].cosphi) * factor;
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
