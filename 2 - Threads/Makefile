# Author: Georgios Manos
# Academic ID: 4333
# E-mail: csd4333@csd.uoc.gr

all: nr ferry

BUILD_DIR = bin
CC = gcc
CFLAGS = -Wall -ansi -pedantic -g -pthread

nr:
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) src/ask1.c -o bin/$@ 
	@printf "\n-> narrow road program created successfully. Execute using ./bin/nr -c <car_count>\n\n"

ferry:
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) src/ask2.c -o bin/$@ 
	@printf "\n-> Ferry program created successfully. Execute using ./bin/ferry -c <car_count>\n\n"

clean:
	@rm -r bin
	@clear
