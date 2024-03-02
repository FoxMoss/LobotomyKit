COMPILER = clang++
EXE = webcore 
LIBS = -llitehtml -lgumbo -lraylib -lstdc++ -std=c++20
FLAGS =-Wall 
SOURCES = $(wildcard ./*.cpp)
OBJ_DIR = obj
OBJECTS = $(patsubst ./%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
LIBRARY_PATH = /usr/local/lib/

all: $(EXE)

$(EXE): $(OBJECTS) 
	$(COMPILER) -o $(EXE) $(DEBUG) $(OBJECTS) $(FLAGS) $(LIBS)
$(OBJ_DIR)/%.o: ./%.cpp
	$(COMPILER) -c -o $@ $< $(FLAGS) $(DEBUG)
