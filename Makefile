!include <shared.mak>


## VARIABLES ##


# Test executable
TEST_EXEC = $(OUT_DIR)\test_$(PROJECT_NAME).exe

# Tests files
TEST_SRC = $(TEST_DIR)\board.c \
           $(TEST_DIR)\sdlife.c
# nmake doesn't handle file name prefixes in inference rule syntax, only directories; so
# the project structure is changed here wrt. GNU/nux: test object files go to a designed
# subdir in the objects directory
TEST_OBJ = $(patsubst %.c,$(OBJ_DIR)\\%.obj,$(TEST_SRC))
# Necessary to avoid redefinition of main()
TEST_REQUIRED_OBJ = $(OBJ_DIR)\grid.obj $(OBJ_DIR)\rules.obj
TEST_LOG = test.log


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
      $(SRC_DIR)\timer.c
OBJ = $(patsubst $(SRC_DIR)\\%.c,$(OBJ_DIR)\\%.obj,$(SRC))

# Output executable
EXEC = $(OUT_DIR)\$(PROJECT_NAME).exe

# Debugging symbols
PDB_FILE = $(PROJECT_NAME).pdb

# Resource files (source and compiled)
RC_FILE = $(PROJECT_NAME).rc
RES_FILE = $(PROJECT_NAME).res


# Preprocessor flags
CPPFLAGS = /I$(INC_DIR) /DICONSIZE=64
# Compilation flags
CFLAGS = /nologo /std:c11 /Wall

# Linkage flags
LDFLAGS = /nologo
LDLIBS = SDL2.lib

# Translate GCC optimization levels into MSVC equivalent
!if "$(OPTIM_LVL)" == "0"
CFLAGS = $(CFLAGS) /Od
!else if "$(OPTIM_LVL)" == "1"
CFLAGS = $(CFLAGS) /Ot
!else if "$(OPTIM_LVL)" == "s"
CFLAGS = $(CFLAGS) /O1
!else if "$(OPTIM_LVL)" == "2"
CFLAGS = $(CFLAGS) /Ox
!else if "$(OPTIM_LVL)" == "3"
CFLAGS = $(CFLAGS) /O2 /Ob3
!endif

!if "$(DEBUG)" == "y"
CPPFLAGS = $(CPPFLAGS) /D_DEBUG
CFLAGS = $(CFLAGS) /Zi /Fd$(PDB_FILE)
LDFLAGS = $(LDFLAGS) /debug
!else
CPPFLAGS = $(CPPFLAGS) /DNDEBUG
LDFLAGS = $(LDFLAGS) /release
!endif



## RULES ##


# Defines the file names extensions for file-wise compilation
.SUFFIXES: .c .obj .rc .res


# Linkage (has to be first to be the default rule)
$(EXEC): $(OBJ) $(OBJ_DIR)\$(RES_FILE)
	@if not exist $(OUT_DIR) md $(OUT_DIR)
	link $(LDFLAGS) /out:$(EXEC) $** $(LDLIBS)

# Filewise compilation
{$(SRC_DIR)}.c{$(OBJ_DIR)}.obj:
	@if not exist $(OBJ_DIR) md $(OBJ_DIR)
	$(CC) /Fo$@ /c $< $(CPPFLAGS) $(CFLAGS)

# Tests compilation
{$(TEST_DIR)}.c{$(OBJ_DIR)\$(TEST_DIR)}.obj:
	@echo Compiling test $@ from $<
	@if not exist $(OBJ_DIR) md $(OBJ_DIR)
	$(CC) /c $< /Fo$@ $(CPPFLAGS) $(CFLAGS)

# Resource compilation
{$(DATA_DIR)}.rc{$(OBJ_DIR)}.res:
	@echo Compiling resource file $< to $@
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
	@if not exist $(OUT_DIR) md $(OUT_DIR)
	link $(LDFLAGS) /out:$(TEST_EXEC) $** $(LDLIBS)
	.\$(TEST_EXEC)

# Remove test build files
testclean:
	@if exist $(OBJ_DIR)\$(TEST_DIR) rmdir /s /q $(OBJ_DIR)\$(TEST_DIR)
	@if exist $(TEST_EXEC) del /f /q $(TEST_EXEC)
	@if exist $(TEST_LOG) del /f /q  $(TEST_LOG)
