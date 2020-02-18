#
# Makefile
# weirdo, 2019-03-14 19:08

OBJ_DIR = objs
BIN = bin
CPPFLAGS = -std=c++14 -Wall
LIBS = -lpthread

run: all
	@echo "Makefile needs your attention"
	clear && ./bin/rsa_out
#&& ./bin/rsa_out && ./bin/rsa_out

all: bin/rsa_out
$(BIN)/rsa_out: $(OBJ_DIR)/rsa.o
	g++ $(CPPFLAGS) -o $@ $(OBJ_DIR)/rsa.o main.cpp $(LIBS)

$(OBJ_DIR)/rsa.o: rsa.hpp rsa.cpp
	g++ $(CPPFLAGS) -g -c -o $@ rsa.cpp $(LIBS)

clean:
	-rm -rf $(OBJ_DIR)/*.o $(BIN)/*_out

# vim:ft=make
#
