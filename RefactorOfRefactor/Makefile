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
CXXFLAGS := -Wall -std=c++23 -I$(HEADERS)
LDFLAGS := $(addprefix -l,$(LIBNAMES)) -L$(LIBRARIES)

.PHONY: all configure build clean

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

clean:
	@rm -r $(BUILD)