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
RELEASE_OBJ = $(patsubst $(SRC_DIR)\\%.c,$(OBJ_DIR)\release\\%.obj,$(SRC))
DEBUG_OBJ = $(patsubst $(SRC_DIR)\\%.c,$(OBJ_DIR)\debug\\%.obj,$(SRC))

# Output executables
RELEASE_EXEC = $(OUT_DIR)\release\$(PROJECT_NAME).exe
DEBUG_EXEC = $(OUT_DIR)\debug\$(PROJECT_NAME).exe

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


# Preprocessor flags
CPPFLAGS = /I$(INC_DIR) /I$(DATA_DIR) /D_CRT_SECURE_NO_WARNINGS /DICONSIZE=64 $(CPPFLAGS)
# Compilation flags
CFLAGS = /nologo /std:c11 /Wall /wd5045 /wd4820 $(CFLAGS)

# Linkage flags
LDFLAGS = /nologo $(LDFLAGS)
LDLIBS = SDL2.lib $(LDLIBS)



## RULES ##


# Defines the file names extensions for file-wise compilation
.SUFFIXES: .c .obj .rc .res


all:
	@echo No default build mode, use either "debug", "release" or "test".
	@exit 1

# Linkage
$(RELEASE_EXEC): $(RELEASE_OBJ) $(RES_FILE)
	@if not exist $(OUT_DIR)\release md $(OUT_DIR)\release
	link /release $(LDFLAGS) /out:$@ $** $(LDLIBS)

$(DEBUG_EXEC): $(DEBUG_OBJ) $(RES_FILE)
	@if not exist $(OUT_DIR)\debug md $(OUT_DIR)\debug
	link /debug /pdb:$(PDB_FILE) $(LDFLAGS) /out:$@ $** $(LDLIBS)

$(TEST_EXEC): $(TEST_OBJ) $(TEST_REQUIRED_OBJ)
	@if not exist $(OUT_DIR)\test md $(OUT_DIR)\test
	link /debug /pdb:$(PDB_FILE) $(LDFLAGS) /out:$(TEST_EXEC) $** $(LDLIBS)

# Filewise compilation
{$(SRC_DIR)}.c{$(OBJ_DIR)\release}.obj:
	@if not exist $(OBJ_DIR)\release md $(OBJ_DIR)\release
	$(CC) /Fo$@ /c $< /DNDEBUG $(CPPFLAGS) $(optim_flags) $(CFLAGS)

{$(SRC_DIR)}.c{$(OBJ_DIR)\debug}.obj:
	@if not exist $(OBJ_DIR)\debug md $(OBJ_DIR)\debug
	$(CC) /Fo$@ /c $< /D_DEBUG $(CPPFLAGS) /Zi /Fd$(PDB_FILE) $(CFLAGS)

# Tests compilation
{$(TEST_DIR)}.c{$(OBJ_DIR)\test}.obj:
	@if not exist $(OBJ_DIR)\test md $(OBJ_DIR)\test
	$(CC) /Fo$@ /c $< D_DEBUG $(CPPFLAGS) /Zi /Fd$(PDB_FILE) $(CFLAGS)

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
test: $(TEST_EXEC)
	.\$(TEST_EXEC)

# Remove test build files
testclean:
	@if exist $(OBJ_DIR)\test rmdir /s /q $(OBJ_DIR)\test
	@if exist $(TEST_EXEC) del /f /q $(TEST_EXEC)
