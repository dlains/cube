.PHONY: clean
.PHONY: test

# Setup the VPATH to the source code and set the SOURCES to just the *.c files.
VPATH        = src
INCLUDE      = include
SOURCES      = $(wildcard $(VPATH)*.c)

# Setup the path to the test source code and set the TEST_SOURCES to just the *.c files.
TEST_DIR     = test
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)

# Setup the build output directories.
BUILD_DIR    = build
DEP_DIR      = build/dep
OBJ_DIR      = build/obj
RESULT_DIR   = build/result

MKDIR        = mkdir -p
EXTENSION    = out

# Setup the Unity test library source directory.
UNITY_DIR    = vendor/unity

# Setup compile variables.
CC           = gcc
DEP_FLAGS    = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.Td
COMPILE.c    = $(CC) $(DEP_FLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.cc   = $(CXX) $(DEP_FLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE  = @mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d && touch $@

CFLAGS       = -g -Wall -std=gnu11 -I$(INCLUDE) -I$(UNITY_DIR) -DTEST

# Rewrite the standard C compile rules to include the dependency files.
%.o : %.c
%.o : %.c $(DEP_DIR)/%.d
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

%.o : %.cc
%.o : %.cc $(DEP_DIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

%.o : %.cxx
%.o : %.cxx $(DEP_DIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

BUILD_PATHS  = $(BUILD_DIR) $(DEP_DIR) $(OBJ_DIR) $(RESULT_DIR)

TEST_RESULTS = $(patsubst $(TEST_DIR)/test_%.c,$(RESULT_DIR)/test_%.txt,$(TEST_SOURCES))

PASSED       = `grep -s PASS $(RESULT_DIR)/*.txt`
FAILED       = `grep -s FAIL $(RESULT_DIR)/*.txt`
IGNORED      = `grep -s IGNORE $(RESULT_DIR)/*.txt`

display:
	@echo "$(TEST_RESULTS)"

test: $(BUILD_PATHS) $(TEST_RESULTS)
	@echo "-----------------------\nIGNORES:\n-----------------------"
	@echo "$(IGNORED)" 
	@echo "-----------------------\nFAILURES:\n----------------------"
	@echo "$(FAILED)"
	@echo "-----------------------\nPASSED:\n------------------------"
	@echo "$(PASSED)" 
	@echo "\nDONE"

clean:
	$(RM) $(OBJ_DIR)/*.o
	$(RM) $(BUILD_DIR)/*.$(EXTENSION)
	$(RM) $(RESULT_DIR)/*.txt

$(RESULT_DIR)/%.txt: $(BUILD_DIR)/%.$(EXTENSION)
	-./$< > $@ 2>&1

$(BUILD_DIR)/test_%.$(EXTENSION): $(OBJ_DIR)/test_%.o $(OBJ_DIR)/%.o $(OBJ_DIR)/unity.o $(DEP_DIR)/test_%.d
	$(LINK) -o $@ $^

$(OBJ_DIR)/%.o:: $(TEST_DIR)/%.c
	$(COMPILE.c) $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o:: $(VPATH)/%.c
	$(COMPILE.c) $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o:: $(UNITY_DIR)/%.c $(UNITY_DIR)/%.h
	$(COMPILE.c) $(CFLAGS) $< -o $@

$(DEP_DIR)/%.d:: $(TEST_DIR)/%.c
	$(DEPEND) $@ $<

$(BUILD_DIR):
	$(MKDIR) $(BUILD_DIR)

$(DEP_DIR):
	$(MKDIR) $(DEP_DIR)

$(OBJ_DIR):
	$(MKDIR) $(OBJ_DIR)

$(RESULT_DIR):
	$(MKDIR) $(RESULT_DIR)

# Don't run any commands on dependency files.
$(DEP_DIR)/%.d: ;

# Tell make to keep these intermediate files around.
.PRECIOUS: $(BUILD_DIR)/test_%.$(EXTENSION)
.PRECIOUS: $(DEP_DIR)/%.d
.PRECIOUS: $(OBJ_DIR)/%.o
.PRECIOUS: $(RESULT_DIR)/%.txt
