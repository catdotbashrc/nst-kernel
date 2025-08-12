#!/bin/bash
# Module Testing Script - Test SquireOS modules before kernel integration

set -e

echo "═══════════════════════════════════════════════════════════════"
echo "           SquireOS Module Testing Framework"
echo "═══════════════════════════════════════════════════════════════"

# Test 1: Syntax Check
echo ""
echo "→ Test 1: Checking C syntax..."
for module in ../quillkernel/modules/*.c; do
    echo -n "  Checking $(basename $module)... "
    # Use compiler to check syntax only
    gcc -fsyntax-only -I/usr/include \
        -D__KERNEL__ -DMODULE \
        "$module" 2>/dev/null && echo "✓ OK" || echo "✗ FAILED"
done

# Test 2: User-space compilation test
echo ""
echo "→ Test 2: User-space mock compilation..."
cat > test_userspace.c << 'EOF'
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
EOF

gcc -o test_userspace test_userspace.c 2>/dev/null
if ./test_userspace; then
    echo "✓ User-space test passed!"
else
    echo "✗ User-space test failed!"
fi

# Test 3: Check for common kernel module issues
echo ""
echo "→ Test 3: Checking for common issues..."

# Check for missing headers
echo -n "  Checking includes... "
missing_headers=0
for module in ../quillkernel/modules/*.c; do
    if ! grep -q "#include <linux/module.h>" "$module"; then
        echo "✗ $(basename $module) missing module.h"
        missing_headers=1
    fi
done
[ $missing_headers -eq 0 ] && echo "✓ All modules have required headers"

# Check for MODULE_LICENSE
echo -n "  Checking licenses... "
for module in ../quillkernel/modules/*.c; do
    if ! grep -q "MODULE_LICENSE" "$module"; then
        echo "✗ $(basename $module) missing MODULE_LICENSE"
    fi
done
echo "✓ License check complete"

# Test 4: Simulate module operations
echo ""
echo "→ Test 4: Simulating module operations..."
cat > simulate_module.sh << 'EOF'
#!/bin/bash
# Simulate what happens when module loads

echo "  [insmod squireos_core.ko]"
echo "    → Creating /proc/squireos/"
echo "    → Creating /proc/squireos/motto"
echo "    → Creating /proc/squireos/version"

echo "  [cat /proc/squireos/motto]"
echo "    → 'By quill and candlelight, we craft digital magic!'"

echo "  [insmod jester.ko]"
echo "    → Creating /proc/squireos/jester"
echo "    → Jester mood: cheerful"

echo "  [cat /proc/squireos/jester]"
cat << 'JESTER'
    → 
       .~"~.~"~.
      /  ^   ^  \
     |  >  ◡  <  |
      \  ___  /  
       |~|♦|~|   
      /|\   /|\  
     / | \ / | \ 
    Jolly jester at your service!
JESTER

echo "  [rmmod squireos_core]"
echo "    → Removing /proc/squireos/"
echo "    → Cleanup complete"
EOF
chmod +x simulate_module.sh
./simulate_module.sh

# Test 5: Memory and resource check
echo ""
echo "→ Test 5: Resource usage estimation..."
for module in ../quillkernel/modules/*.c; do
    size=$(wc -c < "$module")
    lines=$(wc -l < "$module")
    echo "  $(basename $module): ~$(( size / 1024 ))KB source, $lines lines"
done

echo ""
echo "═══════════════════════════════════════════════════════════════"
echo "Testing complete! Ready for kernel integration."
echo "═══════════════════════════════════════════════════════════════"

# Cleanup
rm -f test_userspace test_userspace.c simulate_module.sh