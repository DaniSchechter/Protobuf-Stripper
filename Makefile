CC		:= g++
C_FLAGS := -std=c++17 -pedantic -Wall -Wextra #-Werror 

BIN		:= bin
SRC		:= src
INCLUDE	:= /usr/lib -L /usr/lib

LIBRARIES	:= -lboost_system -lpthread -lboost_thread -lboost_regex 

EXECUTABLE	:= protobuf_stripper

all: $(BIN)/$(EXECUTABLE)

clean:
	$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*
	$(CC) $(C_FLAGS) -I $(INCLUDE) $^ -o $@ $(LIBRARIES)
