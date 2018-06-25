ifeq ($(OS),Windows_NT)
	CLEANUP          = del /F /Q
	MKDIR            = mkdir
	TARGET_EXTENSION = exe
else
	CLEANUP          = rm -f
	MKDIR            = mkdir -p
	TARGET_EXTENSION = out
endif

.PHONY: clean
.PHONY: test

PATH_UNITY   = vendor/unity/
PATH_SOURCE  = src/
PATH_TEST    = test/
PATH_BUILD   = build/
PATH_DEPENDS = build/depends/
PATH_OBJECTS = build/objects/
PATH_RESULTS = build/results/

BUILD_PATHS  = $(PATH_BUILD) $(PATH_DEPENDS) $(PATH_OBJECTS) $(PATH_RESULTS)

SRC_TEST     = $(wildcard $(PATH_TEST)*.c)

CC           = gcc
COMPILE      = $(CC) -c
LINK         = $(CC)
DEPEND       = $(CC) -MM -MG -MF
CFLAGS       = -g -Wall -std=gnu11 -I. -I$(PATH_UNITY) -I$(PATH_SOURCE) -DTEST

RESULTS      = $(patsubst $(PATH_TEST)test_%.c,$(PATH_RESULTS)test_%.txt,$(SRC_TEST))

PASSED       = `grep -s PASS $(PATH_RESULTS)*.txt`
FAILED       = `grep -s FAIL $(PATH_RESULTS)*.txt`
IGNORED      = `grep -s IGNORE $(PATH_RESULTS)*.txt`

test: $(BUILD_PATHS) $(RESULTS)
	@echo "-----------------------\nIGNORES:\n-----------------------"
	@echo "$(IGNORED)" 
	@echo "-----------------------\nFAILURES:\n----------------------"
	@echo "$(FAILED)"
	@echo "-----------------------\nPASSED:\n------------------------"
	@echo "$(PASSED)" 
	@echo "\nDONE"


$(PATH_RESULTS)%.txt: $(PATH_BUILD)%.$(TARGET_EXTENSION)
	-./$< > $@ 2>&1

$(PATH_BUILD)test_%.$(TARGET_EXTENSION): $(PATH_OBJECTS)test_%.o $(PATH_OBJECTS)%.o $(PATH_OBJECTS)unity.o $(PATH_DEPENDS)test_%.d
	$(LINK) -o $@ $^

$(PATH_OBJECTS)%.o:: $(PATH_TEST)%.c
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATH_OBJECTS)%.o:: $(PATH_SOURCE)%.c
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATH_OBJECTS)%.o:: $(PATH_UNITY)%.c $(PATH_UNITY)%.h
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATH_DEPENDS)%.d:: $(PATH_TEST)%.c
	$(DEPEND) $@ $<

$(PATH_BUILD):
	$(MKDIR) $(PATH_BUILD)

$(PATH_DEPENDS):
	$(MKDIR) $(PATH_DEPENDS)

$(PATH_OBJECTS):
	$(MKDIR) $(PATH_OBJECTS)

$(PATH_RESULTS):
	$(MKDIR) $(PATH_RESULTS)

clean:
	$(CLEANUP) $(PATH_OBJECTS)*.o
	$(CLEANUP) $(PATH_BUILD)*.$(TARGET_EXTENSION)
	$(CLEANUP) $(PATH_RESULTS)*.txt

.PRECIOUS: $(PATH_BUILD)test_%.$(TARGET_EXTENSION)
.PRECIOUS: $(PATH_DEPENDS)%.d
.PRECIOUS: $(PATH_OBJECTS)%.o
.PRECIOUS: $(PATH_RESULTS)%.txt


