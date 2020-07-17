CC=g++

LINK=g++

.PHONY : all
all : app

app : main.o
	$(LINK) -o app.exe main.o -lstdc++

main.o : main.cpp
	$(CC) -std=c++17 -Wall -Ofast -c main.cpp -o main.o

.PHONY : clean
clean :
	rm main.o app.exe

.PHONY : test
test :
	echo No Test Available.