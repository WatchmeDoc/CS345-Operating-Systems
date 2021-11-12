
# Author: Georgios Manos
# Academic ID: 4333
# E-mail: csd4333@csd.uoc.gr

all: shell

BUILD_DIR = bin
CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
shell:
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) src/*.c -o bin/$@ -lreadline 
	@printf "\n-> Shell created successfully. Execute using ./bin/shell\n\n"

clean:
	@rm -r bin
	@clear
