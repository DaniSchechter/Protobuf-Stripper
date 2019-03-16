CC		:= g++
C_FLAGS := -std=c++17 -Wall -Wextra

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:= -lboost_system

EXECUTABLE	:= proto_stripper.exe

all: $(BIN)/$(EXECUTABLE)

clean:
	$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*
	$(CC) $(C_FLAGS) -I $(INCLUDE) -L $(LIB) $^ -o $@ $(LIBRARIES)