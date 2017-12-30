# Compilation parameters
DEBUG := n
STATIC := y


# Directories
INC_DIR := inc
SRC_DIR := src
OBJ_DIR := obj
OUT_DIR := out

# Executables
EXEC := sdlife
TEST_EXEC := test_sdlife


# Tests macros
TEST_SRC := $(wildcard $(SRC_DIR)/test*.c)
TEST_OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(TEST_SRC))
TEST_REQ := $(OBJ_DIR)/board.o $(OBJ_DIR)/clop.o $(OBJ_DIR)/rules.o
SRC := $(filter-out $(TEST_SRC), $(wildcard $(SRC_DIR)/*.c))
OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))


# Documentation
DOC_PRG := doxygen
DOC_CFG := Doxyfile
DOC_DIR := doc/


# Compilation flags
CC := gcc
CFLAGS := -std=c11 -pedantic -Wall -Wextra -Wpadded
ifeq ($(DEBUG), y)
	CFLAGS += -g
endif
ifeq ($(STATIC),y)
	LDLIBS := -Wl,-dn -llog -lSDL2 -Wl,-dy -lm -ldl -lasound -lpthread -lpulse \
	          -lsndio -lX11 -lXext -lXcursor -lXinerama -lXi -lXrandr -lXss \
	          -lXxf86vm -lwayland-egl -lwayland-client -lwayland-cursor -lxkbcommon
else
	LDLIBS := -llog -lSDL2
endif
LDFLAGS := -I$(INC_DIR)

TEST_LDLIBS := -llog



.PHONY: all clean distclean doc test testclean

all: $(EXEC)

$(EXEC): $(OBJ)
	mkdir -p $(OUT_DIR)
	$(CC) -o$(OUT_DIR)/$(EXEC) $(OBJ_DIR)/*.o $(CFLAGS) $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o$@ $(LDFLAGS) $(CFLAGS)


clean:
	rm -rf $(OBJ_DIR)

distclean: clean testclean
	rm -rf $(OUT_DIR)
	rm -rf $(DOC_DIR)

doc:
	$(DOC_PRG) $(DOC_CFG)

test: $(TEST_OBJ) $(TEST_REQ)
	$(CC) -o$(OUT_DIR)/$(TEST_EXEC) $(OBJ_DIR)/$(TEST_OBJ) $(OBJ_DIR)/$(TEST_REQ) $(CFLAGS) $(TEST_LDLIBS)
	$(OUT_DIR)/$(TEST_EXEC)

testclean:
	rm -rf $(OBJ_DIR)/$(TEST_OBJ) $(OBJ_DIR)/$(TEST_REQ) $(OUT_DIR)/$(TEST_EXEC)
