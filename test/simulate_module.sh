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
