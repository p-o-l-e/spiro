CXX = g++ #-Wall -Werror -Wextra -pedantic -g
CXXFLAGS := -std=c++23 #-Wall -Werror -Wextra -pedantic -g -fsanitize=address
LDFLAGS =  -fsanitize=address -std=c++23 
COMPILE_FLAGS = #-std=c++23 -Wall -Wextra -g


SRC = core_test.cpp bus.cpp uid.cpp setup/constants.cpp setup/iospecs.cpp modules/lfo.cpp modules/vco.cpp modules/env.cpp modules/node.cpp rack.cpp blueprint.cpp \
modules/vca.cpp modules/pdt.cpp modules/map.cpp modules/rtr.cpp modules/snh.cpp modules/sum.cpp modules/vcd.cpp modules/vcf.cpp modules/mix.cpp spiro.cpp
OBJ = $(SRC:.cc=.o)
EXEC = core_test.run

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

clean:
	rm -rf $(OBJ) $(EXEC)