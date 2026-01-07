# CCP6124 – Space Battle Simulator (C++)

## Overview
This program simulates a space battle between **Zapezoids** and **Rogoatuskans**.  
It loads ship and crew data from CSV files, assigns crew to ships, then runs a round-by-round battle until one faction has no surviving deployed ships.

Key concepts implemented:
- Object-Oriented Programming (encapsulation, inheritance, polymorphism)
- Operator overloading for clean output
- CSV file parsing
- Randomised targeting and battle simulation with “simultaneous fire” resolution

---

## How to Compile
Use standard C++ (no external libraries).

Example (g++):
```bash
g++ -std=c++17 -O2 -o BetelTT9L BetelTT9L.cpp
``` 

Program expects **5-line arguments** in this order:
```bash
./BetelTT9L zShips.csv zCrew.csv rShips.csv rCrew.csv
``` 

## Crew Assignment Rules Implemented
- A ship **cannot be deployed without at least 1 pilot**.
- If a ship requires **2 pilots** but receives **only 1 pilot**, its ability to avoid being hit is reduced:
  - implemented as a **25% increase to its chance to be hit** (e.g., 50% → 62.5%).
- Weapons require the correct crew:
  - without gunners / torpedo handlers, the corresponding weapons **do not fire**.
- Crew are distributed to **man as many ships as possible**:
  - allocate **pilots first** (to deploy max ships),
  - then allocate **second pilots** for 2-pilot ships (if available),
  - then allocate **gunners/torpedo handlers** to activate weapons across as many ships as possible.

---

## Battle Simulation Rules Implemented
- The battle runs in **rounds**.
- Each active weapon selects a **random surviving enemy ship** as a target.
- Hit/miss is determined by the **target ship’s chance to be hit** against that weapon type.
- If hit, the target loses HP based on the weapon’s power.
- Ships are destroyed when HP ≤ 0.
- **Simultaneous fire** is enforced:
  - all attacks for the round are generated first,
  - damage is applied after,
  - ships may still shoot in a round even if destroyed at the end of that round.

---

## Output
Each round prints:
- which ship attacked which ship (and the weapon used),
- hit/miss outcome,
- remaining HP after damage is applied,
- destroyed ships (if any).

At the end, the program prints:
- the winning faction,
- surviving ships and their final HP.

---

## Program Structure
Submission is a **single C++ file**.

Internal organisation is separated into sections:
- Types/Enums/Utilities
- Crew (role definitions and crew records)
- Weapons (weapon types and damage)
- Ships (base `Ship` + derived ship classes per faction/type)
- CSV Parsing (load ships and crew from input files)
- Crew Assignment Engine (assign pilots and weapon crew + deployment checks)
- Battle Engine (round loop, random targeting, simultaneous resolution, logging)
- `main()` (argument handling, loading, assignment, battle start, final results)

Recommended operator overloading:
- `operator<<` for `Ship` (and/or `Fleet`) to print status cleanly.

