#
# Compiler flags
#
CC            = gcc
CFLAGS        = -Wall -Werror -Wextra
CPPFLAGS      = -I include

#
# Project files
#
VPATH         = src
SOURCES       = $(wildcard $(VPATH)/*.c)
OBJECTS       = $(subst .c,.o,$(notdir $(SOURCES))) 
EXE           = cube

#
# Debug build settings
#
DEBUG_DIR     = build/debug
DEBUG_EXE     = $(DEBUG_DIR)/$(EXE)
DEBUG_OBJS    = $(addprefix $(DEBUG_DIR)/, $(OBJECTS))
DEBUG_FLAGS   = -g -O0 -DDEBUG

#
# Release build settings
#
RELEASE_DIR   = build/release
RELEASE_EXE   = $(RELEASE_DIR)/$(EXE)
RELEASE_OBJS  = $(addprefix $(RELEASE_DIR)/, $(OBJECTS))
RELEASE_FLAGS = -O3 -DNDEBUG

.PHONY: all clean debug release test prep

# Default build
all: prep release

#
# Debug rules
#
debug: prep $(DEBUG_EXE)

$(DEBUG_EXE): $(DEBUG_OBJS)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $(DEBUG_EXE) $^

$(DEBUG_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(DEBUG_FLAGS) -o $@ $<

#
# Release rules
#
release: $(RELEASE_EXE)

$(RELEASE_EXE): $(RELEASE_OBJS)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o $(RELEASE_EXE) $^

$(RELEASE_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(RELEASE_FLAGS) -o $@ $<

#
# Utility rules
#
prep:
	@mkdir -p $(DEBUG_DIR) $(RELEASE_DIR) $(RESULTS_DIR)

clean:
	$(RM) $(RELEASE_EXE) $(RELEASE_OBJS) $(DEBUG_EXE) $(DEBUG_OBJS) $(RESULTS_DIR)/*.txt