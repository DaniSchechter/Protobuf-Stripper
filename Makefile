CC		:= gcc
C_FLAGS := -pedantic -ansi -Wall  -Wextra #-Werror

BIN		:= bin
SRC		:= src
INCLUDE	:= include

LIBRARIES	:= -lboost_system -lpthread -lboost_thread

EXECUTABLE	:= proto_stripper.exe

all: $(BIN)/$(EXECUTABLE)

clean:
	$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*
	$(CC) $(C_FLAGS) -I $(INCLUDE) $^ -o $@ $(LIBRARIES)