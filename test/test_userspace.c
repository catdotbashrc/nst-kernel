// Mock kernel functions for testing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KERN_INFO ""
#define printk printf
#define __init
#define __exit

// Mock proc filesystem
struct proc_dir_entry {
    char name[256];
    void *data;
};

struct proc_dir_entry* proc_mkdir(const char* name, void* parent) {
    printf("[MOCK] Creating /proc/%s directory\n", name);
    struct proc_dir_entry* entry = malloc(sizeof(struct proc_dir_entry));
    strcpy(entry->name, name);
    return entry;
}

struct proc_dir_entry* create_proc_read_entry(
    const char* name, 
    int mode, 
    struct proc_dir_entry* parent,
    void* read_func, 
    void* data) {
    printf("[MOCK] Creating /proc/%s/%s (mode: %o)\n", 
           parent ? parent->name : "", name, mode);
    return malloc(sizeof(struct proc_dir_entry));
}

void remove_proc_entry(const char* name, struct proc_dir_entry* parent) {
    printf("[MOCK] Removing /proc/%s/%s\n", 
           parent ? parent->name : "", name);
}

// Include simplified module code
#define MODULE_LICENSE(x)
#define MODULE_AUTHOR(x)
#define MODULE_DESCRIPTION(x)
#define module_init(x) int main() { return x(); }
#define module_exit(x)

// Test the core module logic
static struct proc_dir_entry *squireos_root;

static int squireos_init(void) {
    printk(KERN_INFO "SquireOS: Medieval interface initializing...\n");
    
    squireos_root = proc_mkdir("squireos", NULL);
    if (!squireos_root) {
        printk(KERN_INFO "SquireOS: Failed to create /proc/squireos\n");
        return -1;
    }
    
    // Create motto entry
    create_proc_read_entry("motto", 0444, squireos_root, NULL, NULL);
    
    printk(KERN_INFO "SquireOS: Ready! The jester awaits at /proc/squireos/\n");
    return 0;
}

module_init(squireos_init);
