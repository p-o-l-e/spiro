CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++20 -DDEBUG -g -fsanitize=address
LDFLAGS =  -fsanitize=address

SRC = test.cpp 
OBJ = $(SRC:.cc=.o)
EXEC = test.run

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

clean:
	rm -rf $(OBJ) $(EXEC)