.PHONY:	all test-vp clean


all: libdataflow.so

libdataflow.so: dataflow.cc dataflow.h
	clang++ `llvm-config --cxxflags --ldflags --libs` --shared -o libdataflow.so dataflow.cc

VectorProduct.bc: VectorProduct.c
	clang -emit-llvm -c VectorProduct.c

test-vp: libdataflow.so VectorProduct.bc
	@echo "----------Running dataflow on VectorProduct.c file------------"
	opt --load ./libdataflow.so -dot-dataflow VectorProduct.bc -disable-output
	@echo "-----------------------End of test----------------------------"

clean:
	rm -rf libdataflow.so
