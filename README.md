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
g++ -std=c++17 -O2 -o XyloTT9L XyloTT9L.cpp
