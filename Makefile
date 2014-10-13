all: build/testBinaryTree

build/testBinaryTree: tests/testBinaryTree.cc include/BinaryTree.h include/RedBlackTree.h
	@mkdir -p build
	g++ -g -Wall -Werror -Iinclude tests/testBinaryTree.cc -o build/testBinaryTree
