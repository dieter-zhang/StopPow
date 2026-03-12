
INCLUDE = -I../../src
DIR = ../../src/

UNAME = $(shell uname)
ifeq ($(UNAME), Darwin)
	compiler = clang++
	opts = -c -Wall -stdlib=libc++ -std=c++17 -O3 -I/opt/homebrew/include
	rm = rm
	obj_ext = .o
	LIB_FILE = StopPow.a
	BIN_FILE = StopPow
else ifeq ($(UNAME), Linux)
	compiler = g++
	opts = -c -Wall -fPIC -std=c++11 -O3
	rm = rm
	obj_ext = .o
	LIB_FILE = StopPow.a
	BIN_FILE = StopPow
endif

objects = main$(obj_ext)

$(BIN_FILE): $(objects)
	$(compiler) -o $(BIN_FILE) $(objects) $(LIB_FILE) -L/opt/homebrew/lib -lgsl -lgslcblas
	$(rm) $(objects)

main$(obj_ext): main.cpp
	$(compiler) $(opts) $(INCLUDE) main.cpp

clean:
	rm $(BIN_FILE)