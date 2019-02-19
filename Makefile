all: prog1A prog1B

prog1A: HashTable.o main.o
	g++ -o prog1A main.o HashTable.o
prog1B: HashTable.o main.o
	g++ -o prog1B main.o HashTable.o
