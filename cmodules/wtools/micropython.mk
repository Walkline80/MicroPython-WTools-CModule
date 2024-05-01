WTOOLS_MOD_DIR := $(USERMOD_DIR)

# Add all C files to SRC_USERMOD.
SRC_USERMOD_C += $(WTOOLS_MOD_DIR)/wtools.c
SRC_USERMOD_LIB_C += $(WTOOLS_MOD_DIR)/classes/arraytool.c
SRC_USERMOD_LIB_C += $(WTOOLS_MOD_DIR)/classes/colortool.c

# We can add our module folder to include paths if needed
# This is not actually needed in this example.
CFLAGS_USERMOD += -I$(WTOOLS_MOD_DIR)
CFLAGS_USERMOD += -I$(WTOOLS_MOD_DIR)/classes
