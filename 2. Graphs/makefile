CC          := g++
DEBUG_FLAGS := -g3 
FLAGS       := -lm -Wall -Werror $(DEBUG_FLAGS)

SRC         := ./src
BIN         := ./bin
HEADERS     := ./headers
EXEC        := prog
ZIP         := exercicio.zip

# List of all expected $(BIN)/*.o
ALL         := $(wildcard $(SRC)/*.cpp)

all: $(EXEC)

compile: clean all

run: all
	./$(EXEC)

clean:
	rm -f $(EXEC)

zip:
	rm -f $(ZIP)
	zip $(ZIP) Makefile $(HEADERS)/*.h $(ALL)

$(EXEC): $(ALL)
	$(CC) -o $(EXEC) $(ALL) $(FLAGS)
