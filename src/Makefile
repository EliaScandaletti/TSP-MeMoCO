CXX = g++
CC = g++
CPPFLAGS = -Iinclude -I/opt/cplex/include
CXXFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -L$(CPX_LIBDIR)
LDLIBS = $(CPX_LDFLAGS)

DEPENDENCIES = $(subst .cpp,.d,$(shell find . -name "*.cpp"))

CPX_LIBDIR  = /opt/cplex/lib/x86-64_linux/static_pic
CPX_LDFLAGS = -lcplex -lm -pthread -ldl

OBJ = instance/matrix.o solution/solution.o solution/path.o solver/tabu.o solver/flow.o

all: exact tabu

exact: $(OBJ)

tabu: $(OBJ)

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# include $(DEPENDENCIES)

.PHONY: clean
clean:
		rm -rf $(OBJ) exact tabu
		find . -name "*.d"  -exec rm {} \;

