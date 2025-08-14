# JokerOS Kernel Module Makefile
# Transform thy Nook into a jester's writing device

# Main module object
obj-$(CONFIG_JOKEROS) += jokeros.o

# Component objects that make up the module
jokeros-y := jokeros_core.o

# Optional components based on configuration
jokeros-$(CONFIG_JOKEROS_JESTER) += jester.o
jokeros-$(CONFIG_JOKEROS_TYPEWRITER) += typewriter.o
jokeros-$(CONFIG_JOKEROS_WISDOM) += wisdom.o

# Debug flags if enabled
ifeq ($(CONFIG_JOKEROS_DEBUG),y)
EXTRA_CFLAGS += -DDEBUG -g
endif

# Module information
EXTRA_CFLAGS += -DJOKEROS_VERSION=\"1.0.0\"

# Clean files
clean-files := *.o *.ko *.mod.c .*.cmd