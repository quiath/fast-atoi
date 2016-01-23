
CC = g++
COPT = -Wall -O3 -std=c++11
CNAT = -march=native

.PHONY: clean 

all: fast-atoi-test fast-atoi-test-nat

fast-atoi-test: main.cpp 
	$(CC) $(COPT) main.cpp -o fast-atoi-test

fast-atoi-test-nat: main.cpp
	$(CC) $(COPT) $(CNAT) main.cpp -o fast-atoi-test-nat

clean:
	-rm fast-atoi-test fast-atoi-test-nat

