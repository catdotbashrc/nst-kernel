/*
 * Wisdom Module - Rotating quotes for SquireOS
 * 
 * Provides /proc/squireos/wisdom with writing wisdom from the ages
 * Inspiration for the digital scribe
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("QuillKernel Contributors");
MODULE_DESCRIPTION("Ancient wisdom for modern scribes");

extern struct proc_dir_entry *squireos_root;

static struct proc_dir_entry *wisdom_entry;
static int wisdom_index = 0;

/* Collection of writing wisdom */
static const char *wisdoms[] = {
    "\"The first draft of anything is shit.\" - Ernest Hemingway",
    "\"There is nothing to writing. All you do is sit down and bleed.\" - Hemingway",
    "\"Write drunk, edit sober.\" - Attributed to Hemingway",
    "\"Kill your darlings.\" - William Faulkner",
    "\"The road to hell is paved with adverbs.\" - Stephen King",
    "\"If you don't have time to read, you don't have time to write.\" - Stephen King",
    "\"The scariest moment is always just before you start.\" - Stephen King",
    "\"You can make anything by writing.\" - C.S. Lewis",
    "\"There is no greater agony than bearing an untold story.\" - Maya Angelou",
    "\"A word after a word after a word is power.\" - Margaret Atwood",
    "\"We write to taste life twice.\" - Anaïs Nin",
    "\"The role of a writer is not to say what we can all say, but what we are unable to say.\" - Anaïs Nin",
    "\"Fill your paper with the breathings of your heart.\" - William Wordsworth",
    "\"You must stay drunk on writing so reality cannot destroy you.\" - Ray Bradbury",
    "\"Writing is an exploration. You start from nothing.\" - E.L. Doctorow",
    "\"The pen is mightier than the sword.\" - Edward Bulwer-Lytton",
    "\"I write to discover what I know.\" - Flannery O'Connor",
    "\"Words are a lens to focus one's mind.\" - Ayn Rand",
    "\"Either write something worth reading or do something worth writing.\" - Benjamin Franklin",
    "\"Writing is thinking on paper.\" - William Zinsser",
    "\"By quill and candlelight, we capture eternity.\" - Anonymous Monk, 1287",
    "\"The palest ink is better than the best memory.\" - Chinese Proverb",
    "\"Write what should not be forgotten.\" - Isabel Allende",
    "\"A writer is a world trapped in a person.\" - Victor Hugo",
    "\"To write is human, to edit is divine.\" - Stephen King",
    NULL
};

/* Get next wisdom quote */
static const char *get_next_wisdom(void)
{
    const char *wisdom;
    
    /* Count total wisdoms if not done */
    static int wisdom_count = -1;
    if (wisdom_count == -1) {
        wisdom_count = 0;
        while (wisdoms[wisdom_count])
            wisdom_count++;
    }
    
    /* Get current wisdom and advance index */
    wisdom = wisdoms[wisdom_index];
    wisdom_index = (wisdom_index + 1) % wisdom_count;
    
    return wisdom;
}

/* Display wisdom */
static int wisdom_show(char *buffer, char **start, off_t offset,
                      int count, int *eof, void *data)
{
    int len = 0;
    const char *wisdom = get_next_wisdom();
    
    len = sprintf(buffer, "\n═══ Words of Wisdom ═══\n\n");
    len += sprintf(buffer + len, "%s\n\n", wisdom);
    len += sprintf(buffer + len, "═══════════════════════\n\n");
    
    *eof = 1;
    return len;
}

int wisdom_init(void)
{
    if (!squireos_root) {
        printk(KERN_ERR "Wisdom: SquireOS root not found!\n");
        return -ENOENT;
    }
    
    wisdom_entry = create_proc_read_entry("wisdom", 0444, squireos_root,
                                         wisdom_show, NULL);
    if (!wisdom_entry) {
        printk(KERN_ERR "Wisdom: Failed to create /proc/squireos/wisdom\n");
        return -ENOMEM;
    }
    
    printk(KERN_INFO "Wisdom: Ancient wisdom now available to inspire!\n");
    return 0;
}

void wisdom_exit(void)
{
    if (wisdom_entry)
        remove_proc_entry("wisdom", squireos_root);
    
    printk(KERN_INFO "Wisdom: The sages return to their slumber...\n");
}

EXPORT_SYMBOL(wisdom_init);
EXPORT_SYMBOL(wisdom_exit);