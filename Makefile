
# just modify this line to switch your llvm version
LLVM_CONF=llvm-config

CLANG_BINDIR = $(shell $(LLVM_CONF) --bindir)
CFLAGS = -Wall -Wextra -ggdb -I./
CXXFLAGS = -I./ -Wall -ggdb -Wextra
LLVM_CXXFLAGS = $(shell $(LLVM_CONF) --cxxflags)

# leave empty to prevent error 'xxx is registerd more than once'.
# this is decided by how clang is installed in your system.
LLVM_LIBS = 
# LLVM_LIBS = $(shell $(LLVM_CONF) --libs)
LLVM_LDFLAGS = $(shell $(LLVM_CONF) --ldflags)
LDLIBS = -lm -lpthread -ldl -lncurses

CXX = $(CLANG_BINDIR)/clang
C   = $(CLANG_BINDIR)/clang
OPT = $(CLANG_BINDIR)/opt

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
	$(OPT) -load ./libdataflow.so -dot-dataflow samples/VectorProduct.bc -disable-output
	@echo "-----------------------End of test----------------------------"

clean:
	rm -rf $(OBJS) $(EXES) $(LIBS)

graph: ctrl-data.png

ctrl-data.png: test-vp
	dot -Tpng ctrl-data.dot -o ctrl-data.png

docs:   test-vp graph
	doxygen docs/Doxyfile
