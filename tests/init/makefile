source   := $(realpath ../../src)
target := out.x

cc := $(shell which g++)

compflags :=

flags :=
flags += -fconcepts-diagnostics-depth=3

main:
	${cc} -std=c++20 -g -O3 ${flags} ${compflags} -I${source} main.cc -o ${target}

run: main
	./${target}

clean:
	rm -f ${target}
