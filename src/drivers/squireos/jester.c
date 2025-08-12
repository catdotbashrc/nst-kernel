/*
 * Jester Module - ASCII Art Court Jester for SquireOS
 * 
 * Provides /proc/squireos/jester with dynamic ASCII art based on system state
 * A whimsical companion for lonely writers
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/random.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("QuillKernel Contributors");
MODULE_DESCRIPTION("Court Jester - Whimsical companion for digital scribes");

extern struct proc_dir_entry *squireos_root;  /* From squireos_core.c */

static struct proc_dir_entry *jester_entry;

/* Jester moods based on system state */
enum jester_mood {
    JESTER_HAPPY,
    JESTER_SLEEPY,
    JESTER_EXCITED,
    JESTER_THOUGHTFUL,
    JESTER_MISCHIEVOUS
};

/* Get current jester mood based on time and randomness */
static enum jester_mood get_jester_mood(void)
{
    unsigned long now = jiffies;
    unsigned int rand;
    
    get_random_bytes(&rand, sizeof(rand));
    
    /* Different moods at different times */
    if ((now % 100) < 20)
        return JESTER_SLEEPY;
    else if ((rand % 100) < 10)
        return JESTER_MISCHIEVOUS;
    else if ((rand % 100) < 30)
        return JESTER_THOUGHTFUL;
    else if ((rand % 100) < 60)
        return JESTER_HAPPY;
    else
        return JESTER_EXCITED;
}

/* Display jester ASCII art */
static int jester_show(char *buffer, char **start, off_t offset,
                      int count, int *eof, void *data)
{
    int len = 0;
    enum jester_mood mood = get_jester_mood();
    
    switch (mood) {
    case JESTER_HAPPY:
        len = sprintf(buffer,
            "     .~\"~.~\"~.\n"
            "    /  ^   ^  \\    *jingle jingle*\n"
            "   |  >  ◡  <  |   The jester is happy!\n"
            "    \\  ___  /      Writing brings joy!\n"
            "     |~|♦|~|       \n"
            "     |  |  |       \n"
            "    /|  |  |\\      \n"
            "   /_|__|__|_\\     \n");
        break;
        
    case JESTER_SLEEPY:
        len = sprintf(buffer,
            "     .~\"~.~\"~.\n"
            "    /  -   -  \\    *yawn*\n"
            "   |  >  ~  <  |   The jester grows weary...\n"
            "    \\  ___  /      Perhaps a rest?\n"
            "     |~|♦|~|       zzz...\n");
        break;
        
    case JESTER_EXCITED:
        len = sprintf(buffer,
            "     .~\"~.~\"~.\n"
            "    /  O   O  \\    *JINGLE JANGLE*\n"
            "   |  >  O  <  |   MAGNIFICENT WRITING!\n"
            "    \\  \\o/  /      The words flow like wine!\n"
            "     |~|♦|~|       \n"
            "     |>|||<|       *dancing*\n");
        break;
        
    case JESTER_THOUGHTFUL:
        len = sprintf(buffer,
            "     .~\"~.~\"~.\n"
            "    /  o   -  \\    *hmm...*\n"
            "   |  >  _  <  |   The jester ponders...\n"
            "    \\  ---  /      What tale shall we tell?\n"
            "     |~|♦|~|       \n"
            "     | ? ? |       \n");
        break;
        
    case JESTER_MISCHIEVOUS:
        len = sprintf(buffer,
            "     .~\"~.~\"~.\n"
            "    /  ◔   ◔  \\    *mischievous giggle*\n"
            "   |  >  ω  <  |   The jester has an idea!\n"
            "    \\  ___  /      Write something silly!\n"
            "     |~|♠|~|       \n"
            "     |\\|/\\|       *plot plot plot*\n");
        break;
    }
    
    *eof = 1;
    return len;
}

int jester_init(void)
{
    if (!squireos_root) {
        printk(KERN_ERR "Jester: SquireOS root not found!\n");
        return -ENOENT;
    }
    
    jester_entry = create_proc_read_entry("jester", 0444, squireos_root,
                                          jester_show, NULL);
    if (!jester_entry) {
        printk(KERN_ERR "Jester: Failed to create /proc/squireos/jester\n");
        return -ENOMEM;
    }
    
    printk(KERN_INFO "Jester: The court jester arrives with bells jingling!\n");
    return 0;
}

void jester_exit(void)
{
    if (jester_entry)
        remove_proc_entry("jester", squireos_root);
    
    printk(KERN_INFO "Jester: The jester bows and departs...\n");
}

/* Export for use by other modules */
EXPORT_SYMBOL(jester_init);
EXPORT_SYMBOL(jester_exit);