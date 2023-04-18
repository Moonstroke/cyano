# SPDX-License-Identifier: CECILL-2.1
!include <shared.mak>


## VARIABLES ##


# Test executable
TEST_EXEC = $(OUT_DIR)\test\$(PROJECT_NAME).exe

# Tests files
TEST_SRC = $(TEST_DIR)\bits.c \
           $(TEST_DIR)\cyano.c \
           $(TEST_DIR)\grid.c
TEST_OBJ = $(patsubst $(TEST_DIR)\\%.c,$(OBJ_DIR)\test\\%.obj,$(TEST_SRC))
# Necessary to avoid redefinition of main()
TEST_REQUIRED_OBJ = $(OBJ_DIR)\bits.obj \
                    $(OBJ_DIR)\grid.obj \
                    $(OBJ_DIR)\grid_io.obj \
                    $(OBJ_DIR)\rules.obj


# Variables describing the architecture of the project directory
SRC = $(SRC_DIR)\app.c \
      $(SRC_DIR)\bits.c \
      $(SRC_DIR)\cmdline.c \
      $(SRC_DIR)\file_io.c \
      $(SRC_DIR)\grid.c \
      $(SRC_DIR)\gridwindow.c \
      $(SRC_DIR)\grid_io.c \
      $(SRC_DIR)\main.c \
      $(SRC_DIR)\rules.c \
      $(SRC_DIR)\stringutils.c
OBJ = $(patsubst $(SRC_DIR)\\%.c,$(OBJ_DIR)\\%.obj,$(SRC))

# Output executable
EXEC = $(OUT_DIR)\$(PROJECT_NAME).exe

# Debugging symbols
PDB_FILE = $(PROJECT_NAME).pdb

# Compiled resources files
RES_FILE = $(OBJ_DIR)\$(PROJECT_NAME).res


# Translate GCC optimization levels into MSVC equivalent
optim_flags =
!if "$(OPTIM_LVL)" == "0"
optim_flags = /Od
!else if "$(OPTIM_LVL)" == "1"
optim_flags = /Ot
!else if "$(OPTIM_LVL)" == "s"
optim_flags = /O1
!else if "$(OPTIM_LVL)" == "2"
optim_flags = /Ox
!else if "$(OPTIM_LVL)" == "3"
optim_flags = /O2 /Ob3
!endif

!if "$(DEBUG)" == "y"
cpp_debug_flags = /D_DEBUG
c_debug_flags = /Zi /Fd$(PDB_FILE)
ld_debug_flags = /debug /pdb:$(PDB_FILE)
!else
cpp_debug_flags = /DNDEBUG
c_debug_flags =
ld_debug_flags = /release
!endif


# Preprocessor flags
CPPFLAGS = /I$(INC_DIR) /I$(DATA_DIR) /D_CRT_SECURE_NO_WARNINGS /DICONSIZE=64 $(cpp_debug_flags) $(CPPFLAGS)
# Compilation flags
CFLAGS = /nologo /std:c11 /Wall /wd5045 /wd4820 $(optim_flags) $(c_debug_flags) $(CFLAGS)

# Linkage flags
LDFLAGS = /nologo $(ld_debug_flags) $(LDFLAGS)
LDLIBS = SDL2.lib $(LDLIBS)



## RULES ##


# Defines the file names extensions for file-wise compilation
.SUFFIXES: .c .obj .rc .res


all:
	@echo No default build mode, use either "debug", "release" or "test".
	@exit 1

# Linkage (has to be first to be the default rule)
$(EXEC): $(OBJ) $(RES_FILE)
	@if not exist $(OUT_DIR) md $(OUT_DIR)
	link $(LDFLAGS) /out:$(EXEC) $** $(LDLIBS)

# Filewise compilation
{$(SRC_DIR)}.c{$(OBJ_DIR)}.obj:
	@if not exist $(OBJ_DIR) md $(OBJ_DIR)
	@$(CC) /Fo$@ /c $< $(CPPFLAGS) $(CFLAGS)

# Tests compilation
{$(TEST_DIR)}.c{$(OBJ_DIR)\test}.obj:
	@if not exist $(OBJ_DIR)\test md $(OBJ_DIR)\test
	@$(CC) /c $< /Fo$@ $(CPPFLAGS) $(CFLAGS)

# Resource compilation
{$(DATA_DIR)}.rc{$(OBJ_DIR)}.res:
	@if not exist $(OBJ_DIR) md $(OBJ_DIR)
	rc /nologo /fo $@ $<

# Remove object files
clean:
	@if exist $(PDB_FILE) del /f /q $(PDB_FILE)
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)

# Reset project to initial state
distclean: clean cleandoc
	@if exist $(OUT_DIR) rmdir /s /q $(OUT_DIR)


# (Re)generate doc
doc: $(DOC_CFG)
	$(DOC_PRG) $(DOC_CFG)

# Remove doc directory
cleandoc:
	@if exist $(DOC_DIR) rmdir /s /q $(DOC_DIR)

# Build and launch tests
test: $(TEST_OBJ) $(TEST_REQUIRED_OBJ)
	@if not exist $(OUT_DIR)\test md $(OUT_DIR)\test
	link $(LDFLAGS) /out:$(TEST_EXEC) $** $(LDLIBS)
	.\$(TEST_EXEC)

# Remove test build files
testclean:
	@if exist $(OBJ_DIR)\test rmdir /s /q $(OBJ_DIR)\test
	@if exist $(TEST_EXEC) del /f /q $(TEST_EXEC)
