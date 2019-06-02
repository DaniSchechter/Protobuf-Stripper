CC		:= g++
C_FLAGS := -std=c++17 -pedantic -Wall -Wextra# -DINFO= #-Werror 

BIN		:= bin
SRC		:= src
PROTOMESSAGES := messages
INCLUDE	:= /usr/lib -L /usr/lib

LIBRARIES	:= -lboost_system -lpthread -pthread -lboost_thread -lboost_regex -lcrypto -lssl -lprotobuf

EXECUTABLE	:= protobuf_stripper

all: $(BIN)/$(EXECUTABLE)

clean:
	$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

debug: $(SRC)/* $(PROTOMESSAGES)/*
	$(CC) $(C_FLAGS) -D DEBUG_MODE -I $(INCLUDE) $^ -o $(BIN)/$(EXECUTABLE) $(LIBRARIES)

$(BIN)/$(EXECUTABLE): $(SRC)/* $(PROTOMESSAGES)/*
	$(CC) $(C_FLAGS) -I $(INCLUDE) $^ -o $@ $(LIBRARIES)