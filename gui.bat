@echo off
:: Compile the C++ files 
g++ -fdiagnostics-color=always src/piece.cpp src/move.cpp src/papan.cpp src/state.cpp src/solver.cpp src/main.cpp -o bin/gui.exe

:: Run the Python GUI script
python src\rush_gui.py

:: Pause so window doesn't close immediately
pause