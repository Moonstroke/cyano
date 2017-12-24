# Compilation parameters
DEBUG := n
STATIC := y

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
LDFLAGS :=

TEST_LDLIBS := -llog


# Executables
EXEC := sdlife
TEST_EXEC := test_sdlife

# Tests macros
TEST_SRC := $(wildcard test*.c)
TEST_OBJ := $(TEST_SRC:.c=.o)
TEST_REQ := board.o clop.o rules.o
SRC := $(filter-out $(TEST_SRC), $(wildcard *.c))
OBJ := $(SRC:.c=.o)


# Documentation
DOC_PRG := doxygen
DOC_CFG := Doxyfile
DOC_DIR := doc/


.PHONY: all clean distclean doc test testclean

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o$(EXEC) $(OBJ) $(CFLAGS) $(LDLIBS)

%.o: %.c
	$(CC) -c $< -o$@


clean:
	rm -rf $(OBJ)

distclean: clean testclean
	rm -rf $(EXEC)
	rm -rf $(DOC_DIR)

doc:
	$(DOC_PRG) $(DOC_CFG)

test: $(TEST_OBJ) $(TEST_REQ)
	$(CC) -o$(TEST_EXEC) $(TEST_OBJ) $(TEST_REQ) $(CFLAGS) $(TEST_LDLIBS)
	./$(TEST_EXEC)

testclean:
	rm -rf $(TEST_OBJ) $(TEST_REQ) $(TEST_EXEC)