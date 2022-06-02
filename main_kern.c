//
// Created by lisongqian on 2022/6/2.
//

#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

#define bpf_printk(fmt, ...)                       \
    ({                                             \
        char ____fmt[] = fmt;                      \
        bpf_trace_printk(____fmt, sizeof(____fmt), \
                         ##__VA_ARGS__);           \
    })

struct bpf_map_def SEC("maps") my_map = {
        .type = BPF_MAP_TYPE_HASH,
        .key_size = sizeof(int),
        .value_size = sizeof(int),
        .max_entries = 100,
        .map_flags = BPF_F_NO_PREALLOC,
};

static __always_inline  void bpf_map_kern_example() {

    int key, value, result;
    key = 1, value = 1234;

    result = bpf_map_update_elem(&my_map, &key, &value, BPF_ANY);
    if (result == 0)
        bpf_printk("Map updated with new element\n");
    else {
        bpf_printk("Failed to update map with new value: %d\n", result);
    }


    value = 5678;
    result = bpf_map_update_elem(&my_map, &key, &value, BPF_EXIST);
    if (result == 0)
        bpf_printk("Map updated with new element BPF_EXIST \n");
    else
        bpf_printk("Failed to update map with new value: %d (%s) BPF_EXIST\n", result);

    int read_value;
    read_value = *(int *)bpf_map_lookup_elem(&my_map, &key);
    if (result == 0)
        bpf_printk("Value read from the map: '%d'\n", read_value);
    else
        bpf_printk("Failed to read value from the map : %d (%s)\n", result);

    result = bpf_map_delete_elem(&my_map, &key);
    if (result == 0)
        bpf_printk("Element deleted from the map\n");
    else
        bpf_printk("Failed to delete element from the map : %d (%s)\n", result);
}