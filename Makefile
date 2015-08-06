
CXXFLAGS = -I./ -Wall -ggdb -Wextra
LLVM_CXXFLAGS = $(shell llvm-config --cxxflags)


LLVM_LIBS = $(shell llvm-config --libs)
LLVM_LDFLAGS = $(shell llvm-config --ldflags)

CXX = $(shell which clang++)

OBJS =

OBJS += flow/dataflow.o
OBJS += utils/llvm_utils.o

.PHONY:	all test-vp clean docs

all: libdataflow.so

%.o: %.cc
	$(CXX) -o $@ $(CXXFLAGS) $(LLVM_CXXFLAGS) -c $<

libdataflow.so: $(OBJS)
	$(CXX) --shared -o libdataflow.so $(OBJS) $(LLVM_LDFLAGS) $(LLVM_LIBS)

VectorProduct.bc: VectorProduct.c
	clang -emit-llvm -c VectorProduct.c
dataflow: test.cc flow/dataflow.cc flow/dataflow.h utils/llvm_utils.h utils/llvm_utils.cc
	gcc `llvm-config --cxxflags` test.cc flow/dataflow.cc utils/llvm_utils.cc -o flow/dataflow -lstdc++ `llvm-config --libs all` `llvm-config --ldflags` -lm -lpthread -ldl -lncurses -Wall -ggdb


test-vp: libdataflow.so VectorProduct.bc VectorProduct.c
	@echo "----------Running dataflow on VectorProduct.c file------------"
	opt --load ./libdataflow.so -dot-dataflow VectorProduct.bc -disable-output
	@echo "-----------------------End of test----------------------------"

clean:
	rm -rf libdataflow.so

graph: ctrl-data.png

ctrl-data.png: test-vp
	dot -Tpng ctrl-data.dot -o ctrl-data.png

docs:   test-vp graph
	doxygen docs/Doxyfile


