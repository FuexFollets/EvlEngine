main: main.o
    g++ main.o -o main

main.o: main.cpp
    g++ main.cpp -o main.o

main.cpp:
    echo > src/main.cpp
