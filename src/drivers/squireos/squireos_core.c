/*
 * SquireOS Core Module - Medieval /proc interface for Nook Typewriter
 * 
 * Creates /proc/squireos/ hierarchy for medieval-themed kernel features
 * By quill and candlelight, we code...
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/random.h>

#define SQUIREOS_VERSION "1.0.0"
#define SQUIREOS_CODENAME "Parchment"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("QuillKernel Contributors");
MODULE_DESCRIPTION("SquireOS Medieval Interface for Digital Scribes");
MODULE_VERSION(SQUIREOS_VERSION);

static struct proc_dir_entry *squireos_root;
static struct proc_dir_entry *motto_entry;
static struct proc_dir_entry *version_entry;

/* The eternal motto of our digital scriptorium */
static const char motto[] = "By quill and candlelight, we write eternal words\n";

/* Version information */
static int version_show(char *buffer, char **start, off_t offset, 
                       int count, int *eof, void *data)
{
    int len = 0;
    
    len = sprintf(buffer, "SquireOS %s - Codename: %s\n", 
                  SQUIREOS_VERSION, SQUIREOS_CODENAME);
    len += sprintf(buffer + len, "A medieval writing companion\n");
    len += sprintf(buffer + len, "Kernel: QuillKernel\n");
    
    *eof = 1;
    return len;
}

/* Motto display */
static int motto_show(char *buffer, char **start, off_t offset,
                     int count, int *eof, void *data)
{
    int len = sprintf(buffer, "%s", motto);
    *eof = 1;
    return len;
}

static int __init squireos_init(void)
{
    printk(KERN_INFO "═══════════════════════════════════════════════\n");
    printk(KERN_INFO "    .~\"~.~\"~.\n");
    printk(KERN_INFO "   /  o   o  \\    SquireOS %s initializing...\n", SQUIREOS_VERSION);
    printk(KERN_INFO "  |  >  ◡  <  |   Your faithful digital squire\n");
    printk(KERN_INFO "   \\  ___  /      \n");
    printk(KERN_INFO "    |~|♦|~|       %s\n", motto);
    printk(KERN_INFO "═══════════════════════════════════════════════\n");
    
    /* Create /proc/squireos directory */
    squireos_root = proc_mkdir("squireos", NULL);
    if (!squireos_root) {
        printk(KERN_ERR "SquireOS: Failed to create /proc/squireos\n");
        return -ENOMEM;
    }
    
    /* Create /proc/squireos/motto */
    motto_entry = create_proc_read_entry("motto", 0444, squireos_root,
                                         motto_show, NULL);
    if (!motto_entry) {
        printk(KERN_ERR "SquireOS: Failed to create motto entry\n");
        remove_proc_entry("squireos", NULL);
        return -ENOMEM;
    }
    
    /* Create /proc/squireos/version */
    version_entry = create_proc_read_entry("version", 0444, squireos_root,
                                           version_show, NULL);
    if (!version_entry) {
        printk(KERN_ERR "SquireOS: Failed to create version entry\n");
        remove_proc_entry("motto", squireos_root);
        remove_proc_entry("squireos", NULL);
        return -ENOMEM;
    }
    
    printk(KERN_INFO "SquireOS: Digital scriptorium ready at /proc/squireos\n");
    return 0;
}

static void __exit squireos_exit(void)
{
    remove_proc_entry("version", squireos_root);
    remove_proc_entry("motto", squireos_root);
    remove_proc_entry("squireos", NULL);
    
    printk(KERN_INFO "SquireOS: The scribe rests his quill. Farewell!\n");
}

module_init(squireos_init);
module_exit(squireos_exit);