/*
 * Typewriter Module - Writing Statistics for SquireOS
 * 
 * Tracks keystrokes, words, and writing sessions
 * Provides achievements for dedicated scribes
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/input.h>
#include <linux/notifier.h>
#include <linux/keyboard.h>
#include <linux/time.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("QuillKernel Contributors");
MODULE_DESCRIPTION("Typewriter statistics tracker for digital scribes");

extern struct proc_dir_entry *squireos_root;

static struct proc_dir_entry *typewriter_dir;
static struct proc_dir_entry *stats_entry;
static struct proc_dir_entry *achievement_entry;

/* Writing statistics */
static struct {
    unsigned long keystrokes;
    unsigned long words;
    unsigned long sessions;
    unsigned long total_time;  /* in seconds */
    unsigned long current_session_start;
    unsigned long last_keystroke;
    int in_word;
} stats = {0};

/* Achievement levels based on word count */
static const struct {
    unsigned long words_required;
    const char *title;
    const char *description;
} achievements[] = {
    {0,      "Apprentice Scribe", "Just beginning the journey"},
    {100,    "Novice Writer", "The quill feels natural"},
    {500,    "Journeyman Wordsmith", "Words flow with purpose"},
    {1000,   "Master Scrivener", "A thousand words of wisdom"},
    {5000,   "Literary Artisan", "Crafting tales with skill"},
    {10000,  "Chronicle Keeper", "Ten thousand words recorded"},
    {25000,  "Master of Manuscripts", "A library of thoughts"},
    {50000,  "Sage of the Scriptorium", "Wisdom flows from thy quill"},
    {100000, "Grand Chronicler", "A hundred thousand words eternal"},
    {0, NULL, NULL}
};

/* Get current achievement based on word count */
static int get_achievement_level(void)
{
    int level = 0;
    while (achievements[level + 1].title && 
           stats.words >= achievements[level + 1].words_required) {
        level++;
    }
    return level;
}

/* Keyboard notifier to track keystrokes */
static int typewriter_keyboard_notifier(struct notifier_block *nb,
                                       unsigned long code, void *_param)
{
    struct keyboard_notifier_param *param = _param;
    struct timespec now;
    
    if (code != KBD_KEYCODE || !param->down)
        return NOTIFY_OK;
    
    getnstimeofday(&now);
    
    /* Start new session if idle for more than 5 minutes */
    if (stats.last_keystroke == 0 || 
        (now.tv_sec - stats.last_keystroke) > 300) {
        stats.sessions++;
        stats.current_session_start = now.tv_sec;
    }
    
    stats.keystrokes++;
    stats.last_keystroke = now.tv_sec;
    
    /* Simple word detection: space or enter after characters */
    if (param->value == KEY_SPACE || param->value == KEY_ENTER) {
        if (stats.in_word) {
            stats.words++;
            stats.in_word = 0;
        }
    } else if ((param->value >= KEY_A && param->value <= KEY_Z) ||
               (param->value >= KEY_0 && param->value <= KEY_9)) {
        stats.in_word = 1;
    }
    
    return NOTIFY_OK;
}

static struct notifier_block typewriter_keyboard_nb = {
    .notifier_call = typewriter_keyboard_notifier,
};

/* Display statistics */
static int stats_show(char *buffer, char **start, off_t offset,
                     int count, int *eof, void *data)
{
    int len = 0;
    struct timespec now;
    unsigned long session_time = 0;
    
    getnstimeofday(&now);
    
    if (stats.current_session_start > 0) {
        session_time = now.tv_sec - stats.current_session_start;
    }
    
    len = sprintf(buffer, "═══ Writing Statistics ═══\n");
    len += sprintf(buffer + len, "Keystrokes:     %lu\n", stats.keystrokes);
    len += sprintf(buffer + len, "Words Written:  %lu\n", stats.words);
    len += sprintf(buffer + len, "Sessions:       %lu\n", stats.sessions);
    len += sprintf(buffer + len, "Session Time:   %lu:%02lu\n", 
                  session_time / 60, session_time % 60);
    len += sprintf(buffer + len, "═════════════════════════\n");
    
    *eof = 1;
    return len;
}

/* Display current achievement */
static int achievement_show(char *buffer, char **start, off_t offset,
                           int count, int *eof, void *data)
{
    int len = 0;
    int level = get_achievement_level();
    int next_level = level + 1;
    
    len = sprintf(buffer, "═══ Writing Achievement ═══\n\n");
    len += sprintf(buffer + len, "Current Title:\n");
    len += sprintf(buffer + len, "  ★ %s ★\n", achievements[level].title);
    len += sprintf(buffer + len, "  \"%s\"\n\n", achievements[level].description);
    
    if (achievements[next_level].title) {
        unsigned long words_needed = achievements[next_level].words_required - stats.words;
        len += sprintf(buffer + len, "Next Achievement:\n");
        len += sprintf(buffer + len, "  %s\n", achievements[next_level].title);
        len += sprintf(buffer + len, "  (%lu words to go)\n", words_needed);
    } else {
        len += sprintf(buffer + len, "You have reached the pinnacle!\n");
        len += sprintf(buffer + len, "The Grand Chronicler reigns supreme!\n");
    }
    
    len += sprintf(buffer + len, "\n═══════════════════════════\n");
    
    *eof = 1;
    return len;
}

int typewriter_init(void)
{
    if (!squireos_root) {
        printk(KERN_ERR "Typewriter: SquireOS root not found!\n");
        return -ENOENT;
    }
    
    /* Create /proc/squireos/typewriter directory */
    typewriter_dir = proc_mkdir("typewriter", squireos_root);
    if (!typewriter_dir) {
        printk(KERN_ERR "Typewriter: Failed to create directory\n");
        return -ENOMEM;
    }
    
    /* Create stats entry */
    stats_entry = create_proc_read_entry("stats", 0444, typewriter_dir,
                                         stats_show, NULL);
    if (!stats_entry) {
        printk(KERN_ERR "Typewriter: Failed to create stats entry\n");
        remove_proc_entry("typewriter", squireos_root);
        return -ENOMEM;
    }
    
    /* Create achievement entry */
    achievement_entry = create_proc_read_entry("achievement", 0444, typewriter_dir,
                                              achievement_show, NULL);
    if (!achievement_entry) {
        printk(KERN_ERR "Typewriter: Failed to create achievement entry\n");
        remove_proc_entry("stats", typewriter_dir);
        remove_proc_entry("typewriter", squireos_root);
        return -ENOMEM;
    }
    
    /* Register keyboard notifier */
    register_keyboard_notifier(&typewriter_keyboard_nb);
    
    printk(KERN_INFO "Typewriter: Ready to track thy noble words!\n");
    return 0;
}

void typewriter_exit(void)
{
    unregister_keyboard_notifier(&typewriter_keyboard_nb);
    
    if (achievement_entry)
        remove_proc_entry("achievement", typewriter_dir);
    if (stats_entry)
        remove_proc_entry("stats", typewriter_dir);
    if (typewriter_dir)
        remove_proc_entry("typewriter", squireos_root);
    
    printk(KERN_INFO "Typewriter: %lu words recorded for posterity\n", stats.words);
}

EXPORT_SYMBOL(typewriter_init);
EXPORT_SYMBOL(typewriter_exit);