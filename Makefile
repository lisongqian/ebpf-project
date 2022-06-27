CLANG = clang

EXECABLE = monitor-exec

BPF_CODE = bpf_kern


#BPFTOOLS = /kernel-src/samples/bpf
BPF_LOADER = transplant/bpf_load.c

KERN_INCLUDE += -I/kernel-src/tools/testing/selftests/bpf

USER_INCLUDE += -I/kernel-src/tools/include
USER_INCLUDE += -I/kernel-src/samples/bpf
USER_INCLUDE += -I/kernel-src/tools/perf

LIBRARY_PATH = -L/usr/local/lib64

BPF_SO = -lbpf

COMMON_CODE += common/trace_helpers.c

CFLAGS += $(shell grep -q "define HAVE_ATTR_TEST 1" /kernel-src/tools/perf/perf-sys.h \
                  && echo "-DHAVE_ATTR_TEST=0")

.PHONY: clean $(CLANG) build_main_kern main_user build_bpf_kern bpf_user

clean:
	rm -f *.o *.so $(EXECABLE) main

### 第一章程序

build_bpf_kern: ${BPF_CODE.c} ${BPF_LOADER}
	$(CLANG) -O2 -target bpf -c $(BPF_CODE:=.c) $(KERN_INCLUDE) -o ${BPF_CODE:=.o}


bpf_user: build_bpf_kern
	$(CLANG) $(CFLAGS) -o $(EXECABLE)-1 -lelf $(USER_INCLUDE) $(LIBRARY_PATH) $(BPF_SO) \
        $(BPF_LOADER) $(COMMON_CODE)  bpf_user.c

### 第二章

build_main_kern:main_kern.c ${BPF_LOADER}
	$(CLANG) -O2 -target bpf -c main_kern.c $(KERN_INCLUDE) -o main_kern.o

main_user:
	$(CLANG) $(CFLAGS) -o $(EXECABLE)-2 -lelf $(USER_INCLUDE) $(LIBRARY_PATH) $(BPF_SO) \
        $(BPF_LOADER) $(COMMON_CODE) main_user.c

$(EXECABLE): main_user

.DEFAULT_GOAL := $(EXECABLE)