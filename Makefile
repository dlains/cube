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
# Test files
#
TEST_DIR      = test
TEST_LIB      = vendor/unity
TEST_SOURCES  = $(wildcard $(TEST_DIR)/*.c)
TEST_SOURCES += $(wildcard $(TEST_LIB)/*.c)
TEST_OBJECTS  = $(subst .c,.o,$(notdir $(TEST_SOURCES)))
TEST_OBJECTS += $(subst .c,.o,$(notdir $(SOURCES)))
TEST_RUNNERS  = $(subst .c,,$(notdir $(TEST_SOURCES)))

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

#
# Test build settings
#
RESULTS_DIR   = build/test
CPPFLAGS     += -I $(TEST_LIB)
ALL_RUNNERS   = $(addprefix $(RESULTS_DIR)/, $(TEST_RUNNERS))
TEST_OBJS     = $(addprefix $(RESULTS_DIR)/, $(TEST_OBJECTS))
TEST_RESULTS  = $(patsubst $(TEST_DIR)/test_%.c,$(RESULTS_DIR)/test_%.txt,$(TEST_SOURCES))
PASSED        = `grep -s PASS $(RESULTS_DIR)/*.txt`
FAILED        = `grep -s FAIL $(RESULTS_DIR)/*.txt`
IGNORED       = `grep -s IGNORE $(RESULTS_DIR)/*.txt`

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
# Test rules
#
test: prep $(TEST_RESULTS)
	@echo "-----------------------\nIGNORED:\n-----------------------"
	@echo "$(IGNORED)" 
	@echo "-----------------------\nFAILED:\n------------------------"
	@echo "$(FAILED)"
	@echo "-----------------------\nPASSED:\n------------------------"
	@echo "$(PASSED)" 
	@echo "\nDONE"

$(RESULTS_DIR)/%.txt: $(ALL_RUNNERS)
	-./$< > $@ 2>&1

$(ALL_RUNNERS): $(TEST_OBJS) 
	$(CC) -c $(CFLAGS) -o $@ $^

$(RESULTS_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(TEST_FLAGS) -o $@ $<

#
# Utility rules
#
prep:
	@mkdir -p $(DEBUG_DIR) $(RELEASE_DIR) $(RESULTS_DIR)

clean:
	$(RM) $(RELEASE_EXE) $(RELEASE_OBJS) $(DEBUG_EXE) $(DEBUG_OBJS) $(RESULTS_DIR)/*.txt

# Tell make to keep these intermediate files around.
.PRECIOUS: $(RESULTS_DIR)/%.txt
