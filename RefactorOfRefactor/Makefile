SOURCES := src
HEADERS := hdr
LIBRARIES := lib
BUILD := build

CPPFILES := $(wildcard $(SOURCES)/*.cpp)
LIBFILES := $(wildcard $(LIBRARIES)/*.a)
LIBNAMES := $(patsubst $(LIBRARIES)/lib%.a,%,$(LIBFILES))
OBJFILES := $(patsubst $(SOURCES)/%.cpp,$(BUILD)/%.o,$(CPPFILES))

EXECUTABLE := a.out
CXX := zig c++
CXXFLAGS := -g -Wall -Wextra -Wpedantic -Werror -Wno-missing-field-initializers -std=c++23 -I$(HEADERS)
LDFLAGS := $(addprefix -l,$(LIBNAMES)) -L$(LIBRARIES)

.PHONY: all configure build memory-leaks clean

all: configure build

configure:
	@mkdir -p $(BUILD)

build: configure $(OBJFILES)
	$(CXX) $(OBJFILES) $(LDFLAGS) -o $(EXECUTABLE)

$(BUILD)/%.o: $(SOURCES)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

compile_commands.json: $(wildcard $(BUILD)/*.json)
	$(CXX) $(CXXFLAGS) -gen-cdb-fragment-path $(BUILD) -fsyntax-only $(CPPFILES) || true
	@sed -e '1s|^|[\n|' -e '$$s|,$$|\n]|' $^ > $@

memory-leaks:
	valgrind --gen-suppressions=all --suppressions=valgrind.supp --leak-check=full --show-leak-kinds=all "./$(EXECUTABLE)"

clean:
	@rm -r $(BUILD)
