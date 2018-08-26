#
# Compiler flags
#
CC            = gcc
CFLAGS        = -Wall -Werror -Wextra
CPPFLAGS      = -Iinclude
LDFLAGS       =
LDLIBS        = -lm -lreadline

#
# Homebrew doesn't install Readline to the default location due to library conflicts.
#
ifeq ($(_system_type), Darwin)
CPPFLAGS     += -I/usr/local/Cellar/readline/7.0.3_1/include
LDFLAGS      += -L/usr/local/Cellar/readline/7.0.3_1/lib
else
CPPFLAGS     += -I/usr/include
LDFLAGS      += -L/usr/lib/x86_64-linux-gnu/
endif

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

.PHONY: all clean debug release prep

# Default build
all: prep release

#
# Debug rules
#
debug: prep $(DEBUG_EXE)

$(DEBUG_EXE): $(DEBUG_OBJS)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $(DEBUG_EXE) $^ $(LDFLAGS) $(LDLIBS)

$(DEBUG_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(DEBUG_FLAGS) -o $@ $<

#
# Release rules
#
release: $(RELEASE_EXE)

$(RELEASE_EXE): $(RELEASE_OBJS)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o $(RELEASE_EXE) $^ $(LDFLAGS) $(LDLIBS)

$(RELEASE_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(RELEASE_FLAGS) -o $@ $<

#
# Utility rules
#
prep:
	@mkdir -p $(DEBUG_DIR) $(RELEASE_DIR)

clean:
	$(RM) $(RELEASE_EXE) $(RELEASE_OBJS) $(DEBUG_EXE) $(DEBUG_OBJS)
