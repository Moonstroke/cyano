include shared.mak


## VARIABLES ##


# Test executable
TEST_EXEC := $(OUT_DIR)/test_$(PROJECT_NAME)

# Tests files
TEST_SRC := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ := $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/test_%.o,$(TEST_SRC))
# Necessary to avoid redefinition of main()
TEST_REQUIRED_OBJ := $(OBJ_DIR)/bits.o $(OBJ_DIR)/grid.o $(OBJ_DIR)/grid_io.o $(OBJ_DIR)/rules.o
TEST_LOG := test.log

# Variables describing the architecture of the project directory
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))


# Output executable
EXEC := $(OUT_DIR)/$(PROJECT_NAME)


# Compilation database (used by Sonarlint)
COMPDB := compile_commands.json

# Intermediate compilation command files (one per source file)
CCMD := $(patsubst %.o,%.ccmd,$(OBJ))


# Preprocessor flags
CPPFLAGS := -I$(INC_DIR) -I$(DATA_DIR) -DICONSIZE=64 $(CPPFLAGS)
# Compilation flags
ifeq ($(DEBUG), y)
	debug_flag += -g
endif
CFLAGS := -std=c11 -pedantic -Wall -Wextra $$(sdl2-config --cflags) -O$(OPTIM_LVL) $(debug_flag) $(CFLAGS)

# The libraries to link against
ifeq ($(STATIC),y)
	sdl2_libs=$$(sdl2-config --static-libs)
else
	sdl2_libs = $$(sdl2-config --libs)
endif
LDLIBS := $(sdl2_libs) -lCUTE $(LDLIBS)

# Linkage flags
ifndef ($(LDFLAGS))
	LDFLAGS :=
endif


## RULES ##


# All rule names that do not refer to files
.PHONY: all compdb clean distclean doc cleandoc


# The default rule (the one called when make is invoked without arguments)
all: $(EXEC)

# Linkage
$(EXEC): $(OBJ)
	@mkdir -p $(OUT_DIR)
	$(CC) -o$(EXEC) $^ $(LDFLAGS) $(LDLIBS)

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

# Reset project to initial state
distclean: clean cleandoc
	@rm -rf $(OUT_DIR)


# (Re)generate doc
doc: $(DOC_CFG)
	$(DOC_PRG) $(DOC_CFG)

# Remove doc directory
cleandoc:
	@rm -rf $(DOC_DIR)

# Build and launch tests
test: $(TEST_OBJ) $(TEST_REQUIRED_OBJ)
	@mkdir -p $(OUT_DIR)
	$(CC) -o$(TEST_EXEC) $^ $(LDLIBS) $(LDFLAGS)
	./$(TEST_EXEC)

# Remove test build files
testclean:
	@rm -rf $(TEST_OBJ) $(TEST_EXEC) $(TEST_LOG)
