include shared.mak


## VARIABLES ##


# Test executable
TEST_EXEC := $(OUT_DIR)/test_$(PROJECT_NAME)

# Tests files
TEST_SRC := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ := $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/test_%.o,$(TEST_SRC))
# Necessary to avoid redefinition of main()
TEST_REQUIRED_OBJ := $(OBJ_DIR)/grid.o $(OBJ_DIR)/clop.o $(OBJ_DIR)/rules.o
TEST_LOG := test.log

# Variables describing the architecture of the project directory
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))


# Output executable
EXEC := $(OUT_DIR)/$(PROJECT_NAME)


# Preprocessor flags
CPPFLAGS := -I$(INC_DIR) -DICONSIZE=64
# Compilation flags
CFLAGS := -std=c11 -pedantic -Wall -Wextra $$(sdl2-config --cflags) -O$(OPTIM_LVL)
ifeq ($(DEBUG), y)
	CFLAGS += -g
endif

# The libraries to link against
ifeq ($(STATIC),y)
	LDLIBS := $$(sdl2-config --static-libs)
else
	LDLIBS := $$(sdl2-config --libs)
endif
LDLIBS += -lclog -lCUTE

# Linkage flags
LDFLAGS :=



## RULES ##


# All rule names that do not refer to files
.PHONY: all clean distclean doc cleandoc


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
