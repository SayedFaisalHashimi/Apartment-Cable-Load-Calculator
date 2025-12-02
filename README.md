# Building Electrical Load Calculator

This project is a **clean, structured, and expandable C program** designed to calculate the total electrical load of a building. It collects information about apartments, units (flats), and extra loads such as elevators, pumps, and machinery, then applies diversity factors to produce realistic power estimates.

---

## ✨ Features

* **Interactive input system** for buildings, units, and machines.
* **Clear data structures** for apartments, units, and extra loads.
* **Diversified power calculation** based on engineering principles.
* **Well‑organized output** with formatted summaries.
* **Easily expandable** for future features such as file input, exporting data, or GUI integration.

---

## 📂 Project Structure

```
Building_Loads
├── main.c            # The main code file
├── README.md         # You are reading this
└── (future) data/    # Example input files
```

---

## 🧠 How the Program Works

1. **Building information input**

   * Number of apartments
   * Units per apartment
   * Machines / extra loads

2. **Diversity & demand calculation**

   * Each unit’s kW is diversified with a factor.
   * Machine loads are added as separate loads.
   * All results are summed into a final building load.

3. **Formatted report output**

   * Apartment loads
   * Unit loads
   * Extra loads
   * Total diversified load

---

## 📥 Example Input

```
Enter number of apartments: 3
Enter power (kW) for Apartment 1: 18
Enter number of flats in Apartment 1: 6
...
Enter number of machines: 2
Machine 1 name: Elevator
Power: 5 kW
```

---

## 📤 Example Output

```
-----------------------------------------------
        BUILDING LOAD SUMMARY
-----------------------------------------------
Apartment 1: Diversified Load = 10.80 kW
Apartment 2: Diversified Load = 12.60 kW
Apartment 3: Diversified Load = 09.50 kW

Extra Loads:
  Elevator: 5.00 kW
  Pump:     3.00 kW
-----------------------------------------------
TOTAL BUILDING LOAD = 40.90 kW
-----------------------------------------------
```

---

## 🔧 Requirements

* GCC or any C compiler
* Standard C libraries (`stdio.h`, `stdlib.h`)

Compile with:

```
gcc main.c -o building_load
```

Run with:

```
./building_load
```



## 🔮 Future Improvements

To ensure the long-term reliability, accuracy, and scalability of this electrical load-calculation tool, several enhancements are planned for future releases:

### **1. Advanced Cable Sizing Module**

* Implement automated cable selection based on:

  * Current-carrying capacity (ampacity)
  * Installation method (IEC/NEC tables)
  * Ambient temperature corrections
  * Grouping factors
* Add a cable database (Cu/Al, XLPE/PVC, multi-core, single-core).
* Integrate warnings when cable size is insufficient.

### **2. Voltage Drop Calculation System**

* Add automatic voltage-drop evaluation per feeder.
* Show:

  * % voltage drop
  * Calculated Vdrop per meter
  * Recommended mitigation (larger cable, different route, phase balancing)
* Support for both single-phase and three-phase circuits.

### **3. Transformer Sizing & Selection**

* Implement transformer sizing based on:

  * Total diversified load
  * Power factor
  * Future expansion allowance
  * Harmonics (if nonlinear loads are added)
* Automatic recommendation of transformer kVA rating.

### **4. Harmonic Distortion Analysis (Future Option)**

* Add THD approximation for:

  * Inverters
  * Variable speed drives
  * Nonlinear power supplies
* Suggest mitigation via filters or transformer derating.

### **5. Load Diversity & Demand Factor Enhancements**

* Improve diversified load calculations with configurable:

  * Demand factors
  * Diversity tables
  * Building-type presets (residential, commercial, industrial)

### **6. GUI Interface for Better User Interaction**

* Create a graphical dashboard with:

  * Real-time load visualization
  * Feeder diagrams
  * Cable recommendation checklist
  * Export options (PDF, CSV)

### **7. Automated Reports**

* Generate printable summary reports including:

  * Total load calculation
  * Cable sizing
  * Voltage drop
  * Transformer selection
  * Safety margins and engineering notes

### **8. Error Handling & Data Validation**

* Add robust checks for:

  * Overloaded feeders
  * Invalid input ranges
  * Missing building parameters

### **9. Database Integration**

* Store apartment, load, and machine data in a small SQLite database so users can:

  * Save projects
  * Load previous calculations
  * Compare different design scenarios

### Everybody can use it freely.
