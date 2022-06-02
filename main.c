//
// Created by lisongqian on 2022/6/2.
//
#include <linux/bpf.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "bpf/bpf.h" // /kernel-src/tools/lib/bpf/bpf.h
#include "transplant/bpf_load.h"

int main(int argc, char **argv) {

    int my_map, key, value, result;
    key = 1, value = 1234;
    my_map = bpf_create_map(BPF_MAP_TYPE_HASH, sizeof(int), sizeof(int), 100, BPF_F_NO_PREALLOC);

//    result = bpf_map_update_elem(my_map, &key, &value, BPF_ANY);
    result = bpf_map_update_elem(map_data[0].fd, &key, &value, BPF_ANY);
    if (result == 0)
        printf("Map updated with new element\n");
    else
        printf("Failed to update map with new value: %d (%s)\n", result, strerror(errno));


    value = 5678;
    // create an element with the same key
    result = bpf_map_update_elem(my_map, &key, &value, BPF_NOEXIST);
    if (result == 0)
        printf("Map updated with new element BPF_NOEXIST \n");
    else
        printf("Failed to update map with new value: %d (%s) BPF_NOEXIST\n", result, strerror(errno));

    key = 1;
    int read_value;
    result = bpf_map_lookup_elem(my_map, &key, &read_value);
    if (result == 0)
        printf("Value read from the map: '%d'\n", read_value);
    else
        printf("Failed to read value from the map : %d (%s)\n", result, strerror(errno));

    return 0;
}

