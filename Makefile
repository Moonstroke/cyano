# Compilation parameters
DEBUG := n
STATIC := y

# Compilation flags
CC := gcc
CFLAGS := -finline-functions -pedantic -Wall -Wextra -Wpadded -Wdeclaration-after-statement
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

# File macros
EXEC := sdlife
TEST_FILES := $(wildcard test*.c)
SRC_FILES := $(wildcard *.c)
SRC_FILES := $(filter-out $(TEST_FILES), $(SRC_FILES))
OBJ_FILES := $(SRC_FILES:.c=.o)


# Documentation
DOC_PRG := doxygen
DOC_CFG := Doxyfile
DOC_DIR := doc/


.PHONY: all clean distclean doc

all: $(EXEC)

$(EXEC): $(OBJ_FILES)
	$(CC) -o$(EXEC) $(OBJ_FILES) $(LDLIBS) $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o$@


clean:
	rm -rf $(OBJ_FILES)

distclean: clean
	rm -rf $(EXEC)
	rm -rf $(DOC_DIR)

doc:
	$(DOC_PRG) $(DOC_CFG)
