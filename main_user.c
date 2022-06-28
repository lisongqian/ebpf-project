//
// Created by lisongqian on 2022/6/2.
//
#include <stdio.h>
#include <errno.h>
#include <string.h>

//#include <bpf/bpf.h> // /kernel-src/tools/lib/bpf/bpf.h 用户态
#include <linux//bpf.h>
#include "transplant/bpf_load.h"


static void bpf_map_user_example() {
    int fd, key, value, result;
    key = 1, value = 1234;
    fd = bpf_create_map(BPF_MAP_TYPE_HASH, sizeof(int), sizeof(int), 100, BPF_F_NO_PREALLOC);
    result = bpf_map_update_elem(map_data[0].fd, &key, &value, BPF_ANY); // 仅用户态可用 map_data[0]
    if (result == 0)
        printf("Map updated with new element\n");
    else
        printf("Failed to update map with new value: %d (%s)\n", result, strerror(errno));


    value = 5678;
//    result = bpf_map_update_elem(fd, &key, &value, BPF_EXIST);
    result = bpf_map_update_elem(map_fd[0], &key, &value, BPF_EXIST);
    if (result == 0)
        printf("Map updated with new element BPF_EXIST \n");
    else
        printf("Failed to update map with new value(%d): %d (%s) BPF_EXIST\n", fd, result, strerror(errno));

    int read_value, current_key = -1, next_key;
    while (bpf_map_get_next_key(fd, &current_key, &next_key) == 0) {
        bpf_map_lookup_elem(fd, &next_key, &read_value);    // 读取数据
        printf("%d: %d\n", next_key, read_value);
        bpf_map_delete_elem(fd, &key);
        current_key = next_key;
    }
}

static const char *file_path = "/sys/fs/bpf/my_array";

int save_map2file() {
    int key, value, fd, added, pinned;
    fd = bpf_create_map(BPF_MAP_TYPE_ARRAY, sizeof(int), sizeof(int), 100, 0);
    if (fd < 0) {
        printf("Failed to create map:%d(%s)\n", fd, strerror(errno));
        return -1;
    }
    key = 1, value = 1234;
    added = bpf_map_update_elem(fd, &key, &value, BPF_ANY);
    if (added < 0) {
        printf("Failed to update map:%d(%s)\n", added, strerror(errno));
        return -1;
    }
    pinned = bpf_obj_pin(fd, file_path);
    if (pinned < 0) {
        printf("Failed to pin map to the file system: %d(%s)\n",
               pinned, strerror(errno));
        return -1;
    }
    return 0;
}

int get_map_from_file() {
    int fd, key, value, result;

    fd = bpf_obj_get(file_path);
    if (fd < 0) {
        printf("Failed to fetch the map: %d (%s)\n", fd, strerror(errno));
        return -1;
    }

    key = 1;
    result = bpf_map_lookup_elem(fd, &key, &value);
    if (result < 0) {
        printf("Failed to read value from the map: %d (%s)\n", result,
               strerror(errno));
        return -1;
    }

    printf("Value read from the map: '%d'\n", value);
    return 0;
}

int main(int argc, char **argv) {
    get_map_from_file();
    return 0;
}

