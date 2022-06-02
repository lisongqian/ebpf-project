//
// Created by lisongqian on 2022/6/2.
//
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <bpf/bpf.h> // /kernel-src/tools/lib/bpf/bpf.h

static void bpf_map_user_example() {
    int fd, key, value, result;
    key = 1, value = 1234;
    fd = bpf_create_map(BPF_MAP_TYPE_HASH, sizeof(int), sizeof(int), 100, BPF_F_NO_PREALLOC);
    result = bpf_map_update_elem(fd, &key, &value, BPF_ANY);
    if (result == 0)
        printf("Map updated with new element\n");
    else
        printf("Failed to update map with new value: %d (%s)\n", result, strerror(errno));


    value = 5678;
    result = bpf_map_update_elem(fd, &key, &value, BPF_EXIST);
//    result = bpf_map_update_elem(map_fd[0], &key, &value, BPF_EXIST);
    if (result == 0)
        printf("Map updated with new element BPF_EXIST \n");
    else
        printf("Failed to update map with new value(%d): %d (%s) BPF_EXIST\n", fd, result, strerror(errno));

    int read_value, current_key = -1, next_key;
    while (bpf_map_get_next_key(fd, &current_key, &next_key) == 0) {
        bpf_map_lookup_elem(fd, &next_key, &read_value);
        printf("%d: %d\n", next_key, read_value);
        bpf_map_delete_elem(fd, &key);
        current_key = next_key;
    }
}

int main(int argc, char **argv) {

    bpf_map_user_example();
    return 0;
}

