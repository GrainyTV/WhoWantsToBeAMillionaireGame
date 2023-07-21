# OBJ_SOURCE specifies which files to compile as part of the project
OBJ_SOURCE = main.cpp sources/*.cpp

# OBJ_LIB specifies which headers and libraries to link
OBJ_LIB = -I headers -I SDL/include -Lresources -Wl,-rpath=./resources -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf#-lSDL2_mixer 

# OBJ_NAME specifies the name of our exectuable
OBJ_NAME = WhoWantsToBeAMillionaire

# This is the target that compiles our executable
all : $(OBJ_SOURCE)
	zig c++ $(OBJ_SOURCE) -std=c++2b -fexperimental-library $(OBJ_LIB) -o $(OBJ_NAME)
