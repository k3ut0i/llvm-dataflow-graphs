.PHONY:	all test-vp clean docs


all: libdataflow.so

libdataflow.so: dataflow.cc dataflow.h llvm_utils.h llvm_utils.cc
	clang++ `llvm-config --cxxflags --ldflags --libs` --shared -o libdataflow.so dataflow.cc llvm_utils.cc

VectorProduct.bc: VectorProduct.c
	clang -emit-llvm -c VectorProduct.c

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
