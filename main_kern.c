//
// Created by lisongqian on 2022/6/2.
//

#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
//#include <bpf/bpf.h>

#define bpf_printk(fmt, ...)                       \
    ({                                             \
        char ____fmt[] = fmt;                      \
        bpf_trace_printk(____fmt, sizeof(____fmt), \
                         ##__VA_ARGS__);           \
    })


extern int bpf_create_map(enum bpf_map_type map_type, int key_size,
                              int value_size, int max_entries, __u32 map_flags);

struct concurrent_element {
    struct bpf_spin_lock semaphore;
    int count;
};

struct bpf_map_def SEC("maps") my_map = {
        .type = BPF_MAP_TYPE_HASH,
        .key_size = sizeof(int),
        .value_size = sizeof(int),
        .max_entries = 100,
        .map_flags = BPF_F_NO_PREALLOC,
};

//BPF_ANNOTATE_KV_PAIR(my_map,int, struct struct concurrent_element);

//int bpf_program(struct pt_regs *ctx) {
//    int key = 0;
//    struct concurrent_element init_value = {};
//    struct concurrent_element *read_value;
//    bpf_map_create_elem(&my_map, &key, &init_value, BPF_NOEXIST);
//    read_value = bpf_map_lookup_elem(&my_map, &key);
//    bpf_spin_lock(&read_value->semaphore);
//    read_value->count += 100;
//    bpf_spin_unlock(&read_value->semaphore);
//}

static __always_inline void bpf_map_kern_example() {

    int key, value, result;
    key = 1, value = 1234;

    int fd = bpf_create_map(BPF_MAP_TYPE_HASH, sizeof(int), sizeof(int), 100, BPF_F_NO_PREALLOC);

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