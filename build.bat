@echo off
gcc -static main.c -std=c11 -O3 -s -march=haswell -fno-exceptions -flto ^
 -lmingw32 -lglfw3 -lopengl32 -lgdi32 -ldsound
:: -mwindows
