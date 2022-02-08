LIB_DIR = ./lib
LIB_INC_DIR = $(LIB_DIR)/include

all: main.o
	g++ main.o -o main.exe -lsfml-graphics -lsfml-window -lsfml-system 
	./main.exe

%.o: %.cpp
	g++ -c -std=c++17 main.cpp -I$(LIB_INC_DIR)

clean: 
	rm main.o

.PHONY: all clean