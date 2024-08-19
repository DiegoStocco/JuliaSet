INCLUDE_DIR = include

SOURCES = $(wildcard src/*.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SOURCES))
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

CC = g++
LINKER = g++

COMPILER_FLAGS = -O2 -Wall -g
LINKER_FLAGS = -lsfml_graphics -lsfml_window -lsfml_system

RESULT = julia

all: $RESULT

$(RESULT): $(OBJS) $(HEADERS)
	$(LINKER) $(OBJS) $(LINKER_FLAGS) -o $(RESULT)

%.o : %.cpp
	$(CC) $(addprefix -I, $(INCLUDE_DIR)) $(COMPILER_FLAGS) -c -o $@ $<

.PHONY: clean run
clean:
	rm -f $(OBJS)
	rm -f RESULT

run: $(RESULT)
	./$(RESULT)
