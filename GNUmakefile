# SPDX-License-Identifier: CECILL-2.1
include shared.mak


## VARIABLES ##


# Executables
EXEC := $(OUT_DIR)/$(PROJECT_NAME)
DEBUG_EXEC := $(OUT_DIR)/$(PROJECT_NAME)_debug
TEST_EXEC := $(OUT_DIR)/$(PROJECT_NAME)_test

# Variables describing the architecture of the project directory
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# Tests files
TEST_SRC := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ := $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/test_%.o,$(TEST_SRC))
# Necessary to avoid redefinition of main()
TEST_REQUIRED_OBJ := $(OBJ_DIR)/bits.o \
                     $(OBJ_DIR)/grid.o \
                     $(OBJ_DIR)/grid_io.o \
                     $(OBJ_DIR)/rules.o


# Compilation database (used by Sonarlint)
COMPDB := compile_commands.json

# Intermediate compilation command files (one per source file)
CCMD := $(patsubst %.o,%.ccmd,$(OBJ))


# Preprocessor flags
CPPFLAGS := -I$(INC_DIR) -I$(DATA_DIR) -DICONSIZE=64 $(CPPFLAGS)
# Compilation flags
CFLAGS := -std=c11 -pedantic -Wall -Wextra $$(sdl2-config --cflags) $(CFLAGS)

# The libraries to link against
ifeq ($(STATIC),y)
	sdl2_libs := $$(sdl2-config --static-libs)
else
	sdl2_libs := $$(sdl2-config --libs)
endif
LDLIBS := $(sdl2_libs) $(LDLIBS)

# Linkage flags
ifndef ($(LDFLAGS))
	LDFLAGS :=
endif

# Append build mode-specific compilation flags (optimization level, debug)
# Giving credit where it's due: https://stackoverflow.com/a/1080180/6337519
release: CFLAGS += -O$(OPTIM_LVL) -g1
debug: CFLAGS += -Og -g3
test: CFLAGS += -Og -g3

test: LDLIBS += -lCUTE


## RULES ##


# All rule names that do not refer to files
.PHONY: all release debug compdb clean distclean doc cleandoc


# The default rule (the one called when make is invoked without arguments)
all:
	@echo 'No default build mode. Please specifiy either "debug" or "release".'
	@exit 1

# Build targets
release: $(EXEC)
debug: $(DEBUG_EXEC)

# Build and launch tests
test: $(TEST_OBJ) $(TEST_REQUIRED_OBJ) $(TEST_EXEC)
	./$(TEST_EXEC)


# Linkage
$(EXEC)%: $(OBJ)
	@mkdir -p $(OUT_DIR)
	$(CC) -o$@ $^ $(LDFLAGS) $(LDLIBS)

# Filewise compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -o$@ -c $< $(CPPFLAGS) $(CFLAGS)

# Tests compilation
$(OBJ_DIR)/test_%.o: $(TEST_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS)


# Build the compilation database
# Explanation of the sed invocation:
# All .ccmd files are passed to sed and processed in a continuous text stream
# and the output is redirected to the compilation database file.
# sed runs two commands (separated by the semicolon):
# 1s/^/[\n/
# On the first line, replace the beginning of the line (^ anchor) with an
# opening bracket and a new line. In practice: this injects a [ before the
# first line.
# $s/,$/\n]/
# On the last line (designated by the $ at the start), replace the comma at the
# end of the line with a new line and a closing bracket. Concretely, this
# removes the trailing comma (for JSON compliance) and injects a ] after the
# last line.
$(COMPDB): $(CCMD)
	sed '1s/^/[\n/; $$s/,$$/\n]/' $^ > $@

# Alias to the compilation database file name
compdb: $(COMPDB)

# Intermediate compilation commands files
$(OBJ_DIR)/%.ccmd: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	clang -MJ $@ -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS) $(CFLAGS)


# Remove object files
clean:
	@rm -rf $(OBJ_DIR)
	@rm -f $(COMPDB)

# Reset project to initial state
distclean: clean cleandoc
	@rm -rf $(OUT_DIR)

# Remove test build files
testclean:
	@rm -rf $(TEST_OBJ) $(TEST_EXEC)



# (Re)generate doc
doc: $(DOC_CFG)
	$(DOC_PRG) $(DOC_CFG)

# Remove doc directory
cleandoc:
	@rm -rf $(DOC_DIR)
