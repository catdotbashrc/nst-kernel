# SquireOS Kernel Module Makefile
# Transform thy Nook into a medieval writing device

# Main module object
obj-$(CONFIG_SQUIREOS) += squireos.o

# Component objects that make up the module
squireos-y := squireos_core.o

# Optional components based on configuration
squireos-$(CONFIG_SQUIREOS_JESTER) += jester.o
squireos-$(CONFIG_SQUIREOS_TYPEWRITER) += typewriter.o
squireos-$(CONFIG_SQUIREOS_WISDOM) += wisdom.o

# Debug flags if enabled
ifeq ($(CONFIG_SQUIREOS_DEBUG),y)
EXTRA_CFLAGS += -DDEBUG -g
endif

# Module information
EXTRA_CFLAGS += -DSQUIREOS_VERSION=\"1.0.0\"

# Clean files
clean-files := *.o *.ko *.mod.c .*.cmd