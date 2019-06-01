CC		:= g++
C_FLAGS := -std=c++17 -pedantic -Wall -Wextra# -DINFO= #-Werror 

BIN		:= bin
SRC		:= src
INCLUDE	:= /usr/lib -L /usr/lib

LIBRARIES	:= -lboost_system -lpthread -lboost_thread -lboost_regex -lcrypto -lssl

EXECUTABLE	:= protobuf_stripper

all: $(BIN)/$(EXECUTABLE)

clean:
	$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

debug: $(SRC)/*
<<<<<<< HEAD
	$(CC) $(C_FLAGS) -D DEBUG_MODE -I $(INCLUDE) $^ -o $(BIN)/$(EXECUTABLE) $(LIBRARIES) -g
=======
	$(CC) $(C_FLAGS) -D DEBUG_MODE -I $(INCLUDE) $^ -o $(BIN)/$(EXECUTABLE) $(LIBRARIES)
>>>>>>> 3d333b0bd15ef342b1d63f30404eb7188498eb5e

$(BIN)/$(EXECUTABLE): $(SRC)/*
	$(CC) $(C_FLAGS) -I $(INCLUDE) $^ -o $@ $(LIBRARIES)
