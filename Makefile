CLANG = clang

EXECABLE = monitor-exec

BPFCODE = bpf_program


BPFTOOLS = /kernel-src/samples/bpf
BPFLOADER = transplant/bpf_load.c

CCINCLUDE += -I/kernel-src/tools/testing/selftests/bpf

LOADINCLUDE += -I/kernel-src/samples/bpf
LOADINCLUDE += -I/kernel-src/tools/lib
LOADINCLUDE += -I/kernel-src/tools/perf
LOADINCLUDE += -I/kernel-src/tools/include
LIBRARY_PATH = -L/usr/local/lib64
BPFSO = -lbpf

CFLAGS += $(shell grep -q "define HAVE_ATTR_TEST 1" /kernel-src/tools/perf/perf-sys.h \
                  && echo "-DHAVE_ATTR_TEST=0")

.PHONY: clean $(CLANG) bpfload build

clean:
	rm -f *.o *.so $(EXECABLE)

build: ${BPFCODE.c} ${BPFLOADER}
	$(CLANG) -O2 -target bpf -c $(BPFCODE:=.c) $(CCINCLUDE) -o ${BPFCODE:=.o}

bpfload: build
	clang $(CFLAGS) -o $(EXECABLE) -lelf $(LOADINCLUDE) $(LIBRARY_PATH) $(BPFSO) \
        $(BPFLOADER)  common/common.c loader.c

$(EXECABLE): bpfload

.DEFAULT_GOAL := $(EXECABLE)