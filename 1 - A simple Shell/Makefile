
all: shell

BUILD_DIR = /bin
CC = gcc
CFLAGS = -Wall -ansi -pedantic 
shell:
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) src/*.c -o bin/$@ -lreadline 
	@printf "\n-> Shell created successfully. Execute using ./bin/shell\n\n"

clean:
	@rm -f bin/shell
	@clear
