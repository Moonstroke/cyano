# SPDX-License-Identifier: CECILL-2.1
include shared.mak


## VARIABLES ##


# Executables
RELEASE_EXEC := $(OUT_DIR)/release/$(PROJECT_NAME)
DEBUG_EXEC := $(OUT_DIR)/debug/$(PROJECT_NAME)
TEST_EXEC := $(OUT_DIR)/test/$(PROJECT_NAME)

# Variables describing the architecture of the project directory
SRC := $(wildcard $(SRC_DIR)/*.c)
RELEASE_OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/release/%.o,$(SRC))
DEBUG_OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/debug/%.o,$(SRC))

# Tests files
TEST_SRC := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ := $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/test/%.o,$(TEST_SRC))
# Necessary to avoid redefinition of main()
TEST_REQUIRED_OBJ := $(OBJ_DIR)/debug/bits.o \
                     $(OBJ_DIR)/debug/grid.o \
                     $(OBJ_DIR)/debug/grid_io.o \
                     $(OBJ_DIR)/debug/rules.o


# Compilation database (used by Sonarlint)
COMPDB := compile_commands.json

# Intermediate compilation command files (one per source file)
CCMD := $(patsubst $(OBJ_DIR)/debug/%.o,$(OBJ_DIR)/%.ccmd,$(DEBUG_OBJ))


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


## RULES ##


# All rule names that do not refer to files
.PHONY: all release debug compdb clean distclean doc cleandoc


# The default rule (the one called when make is invoked without arguments)
all:
	@echo 'No default build mode. Please specifiy either "debug" or "release".'
	@exit 1

# Build targets
release: $(RELEASE_EXEC)
debug: $(DEBUG_EXEC)
# Test build also runs the tests
test: $(TEST_EXEC)
	./$(TEST_EXEC)


# Linkage
$(RELEASE_EXEC): $(RELEASE_OBJ)
	@mkdir -p $(OUT_DIR)/release
	$(CC) -o$@ $^ $(LDFLAGS) $(LDLIBS)

$(DEBUG_EXEC): $(DEBUG_OBJ)
	@mkdir -p $(OUT_DIR)/debug
	$(CC) -o$@ $^ $(LDFLAGS) $(LDLIBS)

$(TEST_EXEC): $(TEST_OBJ) $(TEST_REQUIRED_OBJ)
	@mkdir -p $(OUT_DIR)/test
	$(CC) -o$@ $^ $(LDFLAGS) -lCUTE $(LDLIBS)

# Filewise compilation
$(OBJ_DIR)/release/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/release
	$(CC) -o$@ -c $< $(CPPFLAGS) -O$(OPTIM_LVL) -g1 $(CFLAGS)

$(OBJ_DIR)/debug/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/debug
	$(CC) -o$@ -c $< $(CPPFLAGS) -Og -g3 $(CFLAGS)

# Tests compilation
$(OBJ_DIR)/test/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/test
	$(CC) -c $< -o $@ $(CPPFLAGS) -Og -g3 $(CFLAGS)


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
	@mkdir -p $(OBJ_DIR)/debug
	clang -MJ $@ -c $< -o $(OBJ_DIR)/debug/$*.o $(CPPFLAGS) -Og -g3 $(CFLAGS)


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
