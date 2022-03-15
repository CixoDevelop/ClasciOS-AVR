#
# Plik Makefile dla ClasciOS
#
# Do kompilacji mmcu nalezy sprecyzowac
# w parametrze MMCU
#
# Natomiast do wgrania programu nalezy
# tez sprecyzowac programator parametrem
# PROGRAMMER
#
# Autor: Cixo
# Licencja: GNU
#

SOURCE = Test.cpp
COMPILED = Test.bin
CLASCI_SRC = $(wildcard Clasci/*.cpp)
CLASCI_OBJ = $(patsubst %.cpp, %.o, $(CLASCI_SRC))
DRIVERS_SRC = $(wildcard Drivers/*.cpp)
DRIVERS_OBJ = $(patsubst %.cpp, %.o, $(DRIVERS_SRC))
PLATFORM_SRC = Platforms/$(MMCU).cpp
PLATFORM_OBJ = Platforms/$(MMCU).o

SHELL = /bin/sh

CXXFLAGS += -std=c++11 -I. -Os -g -Wall -mmcu=$(MMCU) -mint8
DUDEFLAGS += -c $(PROGRAMMER) -p $(MMCU)
SIZEFLAGS += --mcu=$(MMCU) -C

CC = avr-g++
DUDE = avrdude
SIZE = avr-size
DELETE = rm -f
MOVE = mv

All: Clean Compile Size

Clasci: $(CLASCI_OBJ)

$(CLASCI_OBJ): %.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

Drivers: $(DRIVERS_OBJ)

$(DRIVERS_OBJ): %.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

Platform:
	$(CC) $(CXXFLAGS) -c $(PLATFORM_SRC) -o $(PLATFORM_OBJ)

Compile: Clasci Platform Drivers
	$(CC) $(CXXFLAGS) -o $(COMPILED) $(CLASCI_OBJ) $(DRIVERS_OBJ) $(PLATFORM_OBJ) $(SOURCE)

.PHONY: Clean

Clean:
	$(DELETE) $(PLATFORM_OBJ)
	$(DELETE) $(DRIVERS_OBJ)
	$(DELETE) $(CLASCI_OBJ)
	$(DELETE) $(COMPILED)

Size:
	$(SIZE) $(SIZEFLAGS) $(COMPILED)

Burn:
	$(DUDE) $(DUDEFLAGS) -U flash:w:$(COMPILED)
