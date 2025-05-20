@echo off
g++ -fdiagnostics-color=always src/piece.cpp src/move.cpp src/papan.cpp src/state.cpp src/solver.cpp src/main.cpp -o bin/main.exe && bin\main.exe