/*
 * JokerOS Core Module - Jester's /proc interface for Nook Typewriter
 * 
 * Creates /proc/jokeros/ hierarchy for jester-themed kernel features
 * The jest begins here...
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/random.h>

#define JOKEROS_VERSION "1.0.0"
#define JOKEROS_CODENAME "Laughing Scroll"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JoKernel Contributors");
MODULE_DESCRIPTION("JokerOS Jester Interface for Digital Jesters");
MODULE_VERSION(JOKEROS_VERSION);

static struct proc_dir_entry *jokeros_root;
static struct proc_dir_entry *motto_entry;
static struct proc_dir_entry *version_entry;

/* The eternal jest of our digital court */
static const char motto[] = "In jest we code, in laughter we debug\n";

/* Version information */
static int version_show(char *buffer, char **start, off_t offset, 
                       int count, int *eof, void *data)
{
    int len = 0;
    
    len = snprintf(buffer, PAGE_SIZE, "JokerOS %s - Codename: %s\n", 
                  JOKEROS_VERSION, JOKEROS_CODENAME);
    len += snprintf(buffer + len, PAGE_SIZE - len, "A jester's writing companion\n");
    len += snprintf(buffer + len, PAGE_SIZE - len, "Kernel: JoKernel\n");
    
    *eof = 1;
    return len;
}

/* Motto display */
static int motto_show(char *buffer, char **start, off_t offset,
                     int count, int *eof, void *data)
{
    int len = snprintf(buffer, PAGE_SIZE, "%s", motto);
    *eof = 1;
    return len;
}

static int __init jokeros_init(void)
{
    printk(KERN_INFO "===============================================\n");
    printk(KERN_INFO "    .~\"~.~\"~.\n");
    printk(KERN_INFO "   /  o   o  \\    JokerOS %s initializing...\n", JOKEROS_VERSION);
    printk(KERN_INFO "  |  >  ^  <  |   Your merry digital jester\n");
    printk(KERN_INFO "   \\  ___  /      \n");
    printk(KERN_INFO "    |~|*|~|       %s\n", motto);
    printk(KERN_INFO "===============================================\n");
    
    /* Create /proc/jokeros directory */
    jokeros_root = proc_mkdir("jokeros", NULL);
    if (!jokeros_root) {
        printk(KERN_ERR "JokerOS: Failed to create /proc/jokeros\n");
        return -ENOMEM;
    }
    
    /* Create /proc/squireos/motto */
    motto_entry = create_proc_read_entry("motto", 0444, jokeros_root,
                                         motto_show, NULL);
    if (!motto_entry) {
        printk(KERN_ERR "JokerOS: Failed to create motto entry\n");
        remove_proc_entry("jokeros", NULL);
        return -ENOMEM;
    }
    
    /* Create /proc/squireos/version */
    version_entry = create_proc_read_entry("version", 0444, jokeros_root,
                                           version_show, NULL);
    if (!version_entry) {
        printk(KERN_ERR "JokerOS: Failed to create version entry\n");
        remove_proc_entry("motto", jokeros_root);
        remove_proc_entry("jokeros", NULL);
        return -ENOMEM;
    }
    
    printk(KERN_INFO "JokerOS: Jester's court ready at /proc/jokeros\n");
    return 0;
}

static void __exit jokeros_exit(void)
{
    remove_proc_entry("version", jokeros_root);
    remove_proc_entry("motto", jokeros_root);
    remove_proc_entry("jokeros", NULL);
    
    printk(KERN_INFO "JokerOS: The jester's final bow. Until we jest again!\n");
}

module_init(jokeros_init);
module_exit(jokeros_exit);