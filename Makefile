COMPILER = clang++
EXE = webcore
LIBS =  -lraylib -lgumbo -llitehtml -lstdc++ -std=c++20
FLAGS =-Wall 
SOURCES = $(wildcard ./*.cpp)
OBJ_DIR = obj
BUILD_DIR = build
OBJECTS = $(patsubst ./%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
LIBRARY_PATH = /usr/local/lib/
EXPORT = _loadHtml,_start

all: $(EXE)

wasm:
	em++ -o $(BUILD_DIR)/index.html $(DEBUG) $(SOURCES) $(FLAGS) $(LIBS) --preload-file fonts -I/usr/local/include/ -L./libs/ -s USE_GLFW=3 -DPLATFORM_WEB --shell-file ./shell.html -sEXPORTED_RUNTIME_METHODS=ccall,cwrap -sEXPORTED_FUNCTIONS=$(EXPORT)

$(EXE): $(OBJECTS) 
	$(COMPILER) -o $(BUILD_DIR)/$(EXE) $(DEBUG) $(OBJECTS) $(FLAGS) $(LIBS)
$(OBJ_DIR)/%.o: ./%.cpp
	$(COMPILER) -c -o $@ $< $(FLAGS) $(DEBUG)

clean:
	rm -r build/*
