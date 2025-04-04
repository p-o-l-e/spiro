CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17 -g -fsanitize=address
LDFLAGS =  -fsanitize=address

SRC = test_guts.cpp bus.cpp uid.cpp node.cpp rack.cpp ../setup/blueprint.cpp
OBJ = $(SRC:.cc=.o)
EXEC = test_guts.run

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

clean:
	rm -rf $(OBJ) $(EXEC)