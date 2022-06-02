#include "transplant/bpf_load.h"
#include "common/trace_helpers.h"
#include <stdio.h>

int main(int argc, char **argv) {
    if (load_bpf_file("bpf_kern.o")) {
        printf("The kernel didn't load the BPF program\n");
        return -1;
    }
    read_trace_pipe();
    return 0;
}
