.PHONY: all clean

CC=g++
FLAGS=-std=c++14

all: vimtime
OBJS= objects/main.o objects/help.o objects/report.o objects/modify.o
	
vimtime: $(OBJS)
	$(CC) $^ -o $@ $(FLAGS)

objects/%.o: src/%.cpp
	$(CC) -o $@ -c $< $(FLAGS)

clean:
	rm objects/*.o
	find . -type f -perm /111 -delete
