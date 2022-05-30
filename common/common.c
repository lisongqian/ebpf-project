//
// Created by lisongqian on 2022/5/30.
//

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"

#define DEBUGFS "/sys/kernel/debug/tracing/"

void read_trace_pipe(void) {
    int trace_fd;

    trace_fd = open(DEBUGFS "trace_pipe", O_RDONLY, 0);
    if (trace_fd < 0)
        return;

    while (1) {
        static char buf[4096];
        ssize_t sz;

        sz = read(trace_fd, buf, sizeof(buf) - 1);
        if (sz > 0) {
            buf[sz] = 0;
            puts(buf);
        }
    }
}