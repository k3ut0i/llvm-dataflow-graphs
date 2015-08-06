CFLAGS = -Wall -Wextra -ggdb -I./
CXXFLAGS = -I./ -Wall -ggdb -Wextra
LLVM_CXXFLAGS = $(shell llvm-config --cxxflags)


LLVM_LIBS = $(shell llvm-config --libs)
LLVM_LDFLAGS = $(shell llvm-config --ldflags)
LDLIBS = -lm -lpthread -ldl -lncurses

CXX = $(shell which clang++)
C   = $(shell which clang)

OBJS =
EXES =
LIBS =


OBJS += flow/dataflow.o
OBJS += utils/llvm_utils.o

EXES += dataflow

LIBS += libdataflow.so

.PHONY:	all test-vp clean docs

all: libdataflow.so

%.o: %.cc
	$(CXX) -o $@ $(CXXFLAGS) $(LLVM_CXXFLAGS) -c $<

libdataflow.so: $(OBJS)
	$(CXX) --shared -o libdataflow.so $(OBJS) $(LLVM_LDFLAGS) $(LLVM_LIBS)

%.bc: %.c
	$(C) -o $@ $(CFLAGS) -emit-llvm -c $<

dataflow: main.cc $(OBJS)
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) main.cc $(OBJS) -o dataflow $(LLVM_LIBS) $(LLVM_LDFLAGS) $(LDLIBS)


test-vp: libdataflow.so samples/VectorProduct.bc
	@echo "----------Running dataflow on VectorProduct.c file------------"
	opt --load ./libdataflow.so -dot-dataflow samples/VectorProduct.bc -disable-output
	@echo "-----------------------End of test----------------------------"

clean:
	rm -rf $(OBJS) $(EXES) $(LIBS)

graph: ctrl-data.png

ctrl-data.png: test-vp
	dot -Tpng ctrl-data.dot -o ctrl-data.png

docs:   test-vp graph
	doxygen docs/Doxyfile
